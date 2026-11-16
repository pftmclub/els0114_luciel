-- June effect C3z-2 (2026-10-10): run a registry .lua (BOM source or KOG 0x81 bytecode; this is the KOG luajit) in a stub
-- environment and print what it registers. Driven by june2017_re/dungeon_npc_census.py.
--   luajit lua_registry_dump.lua enum  FILE TABLE   -> "K <name> <value>" for every pair of global TABLE after the run
--   luajit lua_registry_dump.lua calls FILE         -> "C <method> <key>=<value> ..." (tab-separated) per g_pX:Method({...}) call
--                                                      ( field-rewards R0: also per DropTable:Method call, the server drop tables )
-- Unknown globals resolve to markers: NPC_UNIT_ID.X -> "NUI:X", OTHER.X -> "E:OTHER.X"; a nested table field prints as "{}".
local mode, path, tname = arg[1], arg[2], arg[3]
local names = setmetatable({}, { __mode = "k" })
local function enumproxy(n)
  local p = setmetatable({}, { __index = function(_, k)
    if n == "NPC_UNIT_ID" then return "NUI:" .. tostring(k) end
    return "E:" .. n .. "." .. tostring(k)
  end })
  names[p] = n
  return p
end
local function flat(v)
  if type(v) == "table" then return "{}" end
  return (tostring(v):gsub("[\t\r\n]", " "))
end
-- field-rewards R0 (2026-11-16): mode "deep" is "calls" plus, after each C line, one "I <key> <index> <k=v>..." line per table row of
-- every table-valued field (<key> is "#" for the array part): a row's scalars, and a list of scalars as "k=a,b,c".
local function deepflat(v)
  if type(v) == "table" then return names[v] or "{}" end
  return flat(v)
end
local function sorted(t)
  local keys = {}
  for k in pairs(t) do keys[#keys + 1] = k end
  table.sort(keys, function(a, b)
    if type(a) == type(b) then return a < b end
    return type(a) == "number"
  end)
  return keys
end
local function listof(v)
  local l = {}
  for _, e in ipairs(v) do l[#l + 1] = deepflat(e) end
  return table.concat(l, ",")
end
local function rowfields(r)
  local out = {}
  for _, kk in ipairs(sorted(r)) do
    local vv = r[kk]
    if type(vv) == "table" and not names[vv] then out[#out + 1] = tostring(kk) .. "=" .. listof(vv)
    else out[#out + 1] = tostring(kk) .. "=" .. deepflat(vv) end
  end
  return out
end
local function deeprows(t)
  for _, k in ipairs(sorted(t)) do
    local v = t[k]
    if type(v) == "table" and not names[v] then
      if type(k) == "number" then
        print(table.concat({ "I", "#", tostring(k), unpack(rowfields(v)) }, "\t"))
      else
        local nested = false
        for _, i in ipairs(sorted(v)) do if type(v[i]) == "table" and not names[v[i]] then nested = true end end
        if nested then
          for _, i in ipairs(sorted(v)) do
            if type(v[i]) == "table" and not names[v[i]] then
              print(table.concat({ "I", tostring(k), tostring(i), unpack(rowfields(v[i])) }, "\t"))
            end
          end
        else
          print(table.concat({ "I", tostring(k), "-", table.concat(rowfields(v), ",") }, "\t"))
        end
      end
    end
  end
end
local function caller(n)
  return setmetatable({}, { __index = function(_, m)
    return function(_, t)
      local out = { "C", m }
      if type(t) == "table" then
        for k, v in pairs(t) do out[#out + 1] = tostring(k) .. "=" .. (mode == "deep" and deepflat(v) or flat(v)) end
      else
        out[#out + 1] = "arg=" .. flat(t)
      end
      print(table.concat(out, "\t"))
      if mode == "deep" and type(t) == "table" then deeprows(t) end
    end
  end })
end
local env = setmetatable({}, { __index = function(t, k)
  local g = _G[k]
  if g ~= nil and k ~= "arg" then return g end
  local p
  if (mode == "calls" or mode == "deep") and type(k) == "string" and (k:sub(1, 2) == "g_" or k == "DropTable") then p = caller(k) else p = enumproxy(k) end
  rawset(t, k, p); return p
end })
local f, err = loadfile(path)
if not f then print("E\t" .. tostring(err)); return end
setfenv(f, env)
local ok, rerr = pcall(f)
if not ok then print("E\t" .. tostring(rerr)) end
if mode == "enum" then
  local t = rawget(env, tname)
  if type(t) ~= "table" then print("E\tno table " .. tostring(tname)); return end
  for k, v in pairs(t) do print("K\t" .. tostring(k) .. "\t" .. tostring(v)) end
end
