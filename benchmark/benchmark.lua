-- benchmark.lua
-- run from the project root: sudo lua benchmark.lua
-- or: lua benchmark.lua (without RT scheduling elevation)

local ROOT  = "."
local TMP   = "/tmp/foil_bench"
local CC    = "gcc"
local FASM  = "fasm"
local CFLAGS = "-O2 -mavx2 -march=native -Wall"

------------------------------------------------------------------------
-- benchmark definitions
-- add new benchmarks here, each entry is:
--   name   : printed label
--   entry  : the main .c file  (relative to ROOT)
--   c      : list of .c sources to compile and link
--   asm    : list of FASM .s sources to compile and link
------------------------------------------------------------------------
local benchmarks = {
    {
        name  = "btree",
        entry = "benchmark/btree.c",
        c = {
            "data-structures/btree/btree.c",
            "debug/print/print.c",
            "syscall/random/random.c",
            "syscall/mem/mem.c",
            "syscall/sched/sched_setscheduler/sched_setscheduler.c",
            "syscall/sched/sched_setaffinity/sched_setaffinity.c",
            "syscall/getcpu/getcpu.c",
            "syscall/clock/clock.c",
            "algorithms/stringify/stringify.c",
        },
        asm = {
            "data-structures/btree/asm/x86-64/get.s",
            "data-structures/btree/asm/x86-64/new.s",
            "data-structures/btree/asm/x86-64/push.s",
            "data-structures/btree/asm/x86-64/remove.s",
        },
    },
    -- add more benchmarks here:
    -- {
    --     name  = "array",
    --     entry = "benchmark/array.c",
    --     c     = { ... },
    --     asm   = {},
    -- },
}

------------------------------------------------------------------------
-- helpers
------------------------------------------------------------------------
local function run(cmd)
    io.write("  $ " .. cmd .. "\n")
    local handle = io.popen(cmd .. " 2>&1", "r")
    local output = handle:read("*a")
    local ok, reason, code = handle:close()
    if output and #output > 0 then
        io.write(output)
    end
    if not ok then
        io.write("[!] command failed (exit " .. tostring(code) .. "): " .. cmd .. "\n")
        os.exit(1)
    end
end

local function obj_path(src)
    -- flatten path to a single filename in TMP
    local flat = src:gsub("/", "_"):gsub("%.%a+$", "")
    return TMP .. "/" .. flat .. ".o"
end

local function compile_c(src)
    local obj = obj_path(src)
    run(string.format("%s %s -c %s/%s -o %s", CC, CFLAGS, ROOT, src, obj))
    return obj
end

local function compile_asm(src)
    local obj = obj_path(src)
    -- fasm outputs a flat object; wrap into ELF64 .o via objcopy
    local tmp_bin = obj .. ".bin"
    run(string.format("%s %s/%s %s", FASM, ROOT, src, obj))
    return obj
end

local function link(objs, out)
    local obj_list = table.concat(objs, " ")
    run(string.format("%s %s -o %s -lc", CC, obj_list, out))
end

------------------------------------------------------------------------
-- main
------------------------------------------------------------------------
run("mkdir -p " .. TMP)

for _, bench in ipairs(benchmarks) do
    io.write("\n[bench] " .. bench.name .. "\n")

    local objs = {}

    -- compile entry point
    table.insert(objs, compile_c(bench.entry))

    -- compile c sources
    for _, src in ipairs(bench.c) do
        table.insert(objs, compile_c(src))
    end

    -- compile fasm sources
    for _, src in ipairs(bench.asm) do
        table.insert(objs, compile_asm(src))
    end

    -- link
    local bin = TMP .. "/" .. bench.name
    link(objs, bin)

    -- run with sudo for SCHED_FIFO elevation
    io.write("\n[run]  " .. bench.name .. "\n")
    run("sudo " .. bin)
end

io.write("\n[done]\n")
