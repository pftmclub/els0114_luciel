-- June effect C3z-2 (2026-10-10): run one dungeon script (stock-header LuaJIT bytecode or source) in a stub environment and print
-- its NPC entries the way the client's loaders walk them (X2Dungeon.cpp STAGE_LIST / STAGE<n>, X2DungeonStage.cpp SUB_STAGE<n>,
-- X2DungeonSubStage.cpp LoadNPCData / New_LoadNPCData / ParseNpcGroupTable: NPC_GROUP[1..], SUB_NPC<i>, NPC_GROUP_RATE + NPC_GROUP<N>).
-- Every global the script reads resolves to a marker: NPC_UNIT_ID.X -> "NUI:X", OTHER.X -> "E:OTHER.X". Driven by
-- june2017_re/dungeon_npc_census.py; prints tab-separated lines on stdout:
--   N <stage> <sub> <form> <group> <index> <subnpc> <npc_id>     form = plain | rate ; subnpc = - or the SUB_NPC index
--   K <stage> <sub> <cond> <index> <npc_id>                      CLEAR_COND<cond>.NPC_ID[index] (CC_KILL_NPC_TYPE)
--   R <name>                                                     every NPC_UNIT_ID name the main chunk reads
--   E <message>                                                  load or run error
-- field-rewards R0 (2026-11-16): with arg[2] == "marks" it also prints (default output is unchanged):
--   M <stage> <sub> <form> <group> <index> <subnpc> <key=value>...   after each N line: the scalar keys of the table that carries the NPC_ID
--   W <stage> <sub> <npc_id> <rate>                                  per sub-stage: the NPC_REWARD_RATE rows ( REWARD_RATE, NPC_ID ) that survive the run
local path = arg[1]
local MARKS = arg[2] == "marks"
local read = {}
local names = setmetatable({}, { __mode = "k" })
local function enumproxy(tname)
  local p = setmetatable({}, { __index = function(_, k)
    if tname == "NPC_UNIT_ID" then read[k] = true; return "NUI:" .. tostring(k) end
    return "E:" .. tname .. "." .. tostring(k)
  end })
  names[p] = tname
  return p
end
local function sv(v)
  if type(v) == "table" then return names[v] or "{}" end
  return (tostring(v):gsub("[\t\r\n]", " "))
end
local function marks(st, sb, form, g, i, j, t)
  local out = { "M", st, sb, form, g, i, j }
  local keys = {}
  for k, v in pairs(t) do if type(k) == "string" and k ~= "NPC_ID" and (type(v) ~= "table" or names[v]) then keys[#keys + 1] = k end end
  table.sort(keys)
  for _, k in ipairs(keys) do out[#out + 1] = k .. "=" .. sv(t[k]) end
  print(table.concat(out, "\t"))
end
local env = setmetatable({}, { __index = function(t, k)
  local g = _G[k]
  if g ~= nil and k ~= "arg" then return g end
  local p = enumproxy(k); rawset(t, k, p); return p
end })
local f, err = loadfile(path)
if not f then print("E\t" .. tostring(err)); return end
setfenv(f, env)
local ok, rerr = pcall(f)
if not ok then print("E\t" .. tostring(rerr)) end

local function idof(t)
  local v = rawget(t, "NPC_ID")
  if v == nil then return "nil" end
  if type(v) == "table" then return "table" end
  return tostring(v)
end
local function group(st, sb, form, g, tbl)
  local i = 1
  while type(tbl[i]) == "table" do
    local e, any, j = tbl[i], false, 0
    while type(e["SUB_NPC" .. j]) == "table" do
      print(table.concat({ "N", st, sb, form, g, i, j, idof(e["SUB_NPC" .. j]) }, "\t")); any = true
      if MARKS then marks(st, sb, form, g, i, j, e["SUB_NPC" .. j]) end
      j = j + 1
    end
    if not any then
      print(table.concat({ "N", st, sb, form, g, i, "-", idof(e) }, "\t"))
      if MARKS then marks(st, sb, form, g, i, "-", e) end
    end
    i = i + 1
  end
end
local SL = rawget(env, "STAGE_LIST")
if type(SL) == "table" then
  local s = 0
  while type(SL["STAGE" .. s]) == "table" do
    local stage, b = SL["STAGE" .. s], 0
    while type(stage["SUB_STAGE" .. b]) == "table" do
      local sub = stage["SUB_STAGE" .. b]
      if type(sub.NPC_GROUP_RATE) == "table" then
        local g = 0
        while type(sub["NPC_GROUP" .. g]) == "table" do group(s, b, "rate", g, sub["NPC_GROUP" .. g]); g = g + 1 end
      elseif type(sub.NPC_GROUP) == "table" then
        group(s, b, "plain", 0, sub.NPC_GROUP)
      end
      -- CLEAR_COND<c>.NPC_ID (CC_KILL_NPC_TYPE): the client reads [1..] until the first nil, so a nil id ends the list
      local c = 0
      while type(sub["CLEAR_COND" .. c]) == "table" do
        local ids = sub["CLEAR_COND" .. c].NPC_ID
        if type(ids) == "table" then
          local i = 1
          while ids[i] ~= nil do print(table.concat({ "K", s, b, c, i, tostring(ids[i]) }, "\t")); i = i + 1 end
        end
        c = c + 1
      end
      if MARKS and type(sub.NPC_REWARD_RATE) == "table" then
        local rr = sub.NPC_REWARD_RATE
        local list = type(rr[1]) == "table" and rr or { rr }
        for _, r in ipairs(list) do
          if type(r) == "table" and r.NPC_ID ~= nil then print(table.concat({ "W", s, b, tostring(r.NPC_ID), sv(r.REWARD_RATE) }, "\t")) end
        end
      end
      print(table.concat({ "B", s, b }, "\t"))
      b = b + 1
    end
    s = s + 1
  end
end
for k in pairs(read) do print("R\t" .. tostring(k)) end
