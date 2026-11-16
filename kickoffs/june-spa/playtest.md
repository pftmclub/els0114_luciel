# june-spa play-test sections

One section per phase, written by that phase at Finish while it knows what it changed. A batch hand-off is assembled from these with
`python tools/playtest.py assemble june-spa [ID ...]`, never rewritten later. `python tools/playtest.py check june-spa` lints them.
Standing rules still apply to every section: dev commands first, copy-ready lines, per check where / what / expect / if not, and the log
lines to read. Batch: S1 and S2a are assembled with S2b at the S2b seam.
