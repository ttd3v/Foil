-- foil build script
-- Assembler: fasm (x86-64 only for now)
-- Compiler:  gcc

-- ---------------------------------------------------------------------------
-- Architecture check
-- ---------------------------------------------------------------------------
function Arch()
        local handle = io.popen("uname -m")
        if handle == nil then return -1 end
        local result = handle:read("*l")
        handle:close()

        if     result == "x86_64"  then return 0
        elseif result == "riscv64" then return 1
        elseif result == "aarch64" then return 2
        else return -1
        end
end

local arch = Arch()
if arch == -1 then
        print("Unsupported architecture.")
        return
end
if arch ~= 0 then
        print("Only x86-64 is currently supported (arm64 and rv64 asm not yet written).")
        return
end

-- ---------------------------------------------------------------------------
-- Helpers
-- ---------------------------------------------------------------------------
local BUILD = "build"
local CC    = "gcc"
local AS    = "fasm"

function run(cmd)
        print("  " .. cmd)
        local ok = os.execute(cmd)
        if not ok then
                print("[ERROR] command failed: " .. cmd)
                os.exit(1)
        end
end

function mkdir(dir)
        os.execute("mkdir -p " .. dir)
end

-- compile a .c file -> .o
-- inc: include flag string, e.g. "-I." or "-I.. -I."
function cc(src, inc)
        inc = inc or "-I."
        local obj = BUILD .. "/" .. src:gsub("/", "_"):gsub("%.c$", ".o")
        run(CC .. " -c -ffreestanding -nostdinc " .. inc .. " -O3 -o " .. obj .. " " .. src)
        return obj
end

-- assemble a fasm .s file -> .o
function asm(src)
        local obj = BUILD .. "/" .. src:gsub("/", "_"):gsub("%.s$", ".o")
        run(AS .. " " .. src .. " " .. obj)
        return obj
end

-- ---------------------------------------------------------------------------
-- Source manifest
-- ---------------------------------------------------------------------------

-- ASM sources keyed by arch (0=x86-64, 1=rv64, 2=aarch64)
local asm_sources = {
        [0] = {
                "data-structures/btree/asm/x86-64/new.s",
                "data-structures/btree/asm/x86-64/push.s",
                "data-structures/btree/asm/x86-64/get.s",
                "data-structures/btree/asm/x86-64/remove.s",
                "atomic/queue/aqueue_x8664.s",
        },
        -- [1] = { },   -- rv64:    TODO
        -- [2] = { },   -- aarch64: TODO
}

local c_sources = {
        -- algorithms
        "algorithms/hash/hash.c",
        "algorithms/lineset/lineset.c",
        "algorithms/parsers/float.c",
        "algorithms/parsers/number.c",
        "algorithms/parsers/unumber.c",
        "algorithms/stringify/stringify.c",
        "syscall/random/random.c",
        -- atomic
        "atomic/queue/aqueue.c",
        -- data-structures
        "data-structures/array/array.c",
        "data-structures/btree/btree.c",
        -- debug
        "debug/ping/ping.c",
        "debug/print/print.c",
        -- syscall
        "syscall/close/close.c",
        "syscall/fstat/fstat.c",
        "syscall/iouring/iouring.c",
        "syscall/mem/mem.c",
        "syscall/mkdir/mkdir.c",
        "syscall/open/open.c",
        "syscall/read/read.c",
        "syscall/write/write.c",
        -- syscall/network
        "syscall/network/accept/accept.c",
        "syscall/network/bind/bind.c",
        "syscall/network/connect/connect.c",
        "syscall/network/listen/listen.c",
        "syscall/network/setsockopt/setsockopt.c",
        "syscall/network/socket/socket.c",
}

-- Test binaries: { source, binary, include flags }
-- tests/test.c uses "../foo/bar.h" style includes, so it needs -I.. (repo root)
local tests = {
        { "tests/test.c", "test", "-I.." },

        -- Older split test files (disabled — enable as needed)
        -- { "tests/syscall.c",         "test_syscalls",       "-I." },
        -- { "tests/algorithms.c",      "test_algorithms",     "-I." },
        -- { "tests/data-structures.c", "test_data_structures","-I." },
}

-- ---------------------------------------------------------------------------
-- Build library
-- ---------------------------------------------------------------------------
mkdir(BUILD)

print("\n=== Assembling (" .. (asm_sources[arch] and #asm_sources[arch] or 0) .. " files) ===")
local asm_objs = {}
if asm_sources[arch] then
        for _, src in ipairs(asm_sources[arch]) do
                table.insert(asm_objs, asm(src))
        end
end

print("\n=== Compiling C library (" .. #c_sources .. " files) ===")
local lib_objs = {}
for _, src in ipairs(c_sources) do
        table.insert(lib_objs, cc(src, "-I."))
end

local all_objs = {}
for _, o in ipairs(asm_objs)  do table.insert(all_objs, o) end
for _, o in ipairs(lib_objs)  do table.insert(all_objs, o) end
local all_objs_str = table.concat(all_objs, " ")

-- ---------------------------------------------------------------------------
-- Build and run tests
-- ---------------------------------------------------------------------------
print("\n=== Compiling and linking tests ===")
local bins = {}
for _, t in ipairs(tests) do
        local src, name, inc = t[1], t[2], t[3]
        local test_obj = cc(src, inc)
        local bin      = BUILD .. "/" .. name
        run(CC .. " -o " .. bin .. " " .. test_obj .. " " .. all_objs_str)
        table.insert(bins, { bin = bin, name = name })
end

print("\n=== Running tests ===\n")

local passed = 0
local failed = 0

for _, t in ipairs(bins) do
        print("--- " .. t.name .. " ---")
        local ok, kind, code = os.execute("./" .. t.bin)
        if ok then
                passed = passed + 1
                print("[OK] " .. t.name .. "\n")
        else
                failed = failed + 1
                print("[FAIL] " .. t.name .. " (" .. tostring(kind) .. " " .. tostring(code) .. ")\n")
        end
end

print("=== Results: " .. passed .. "/" .. (passed + failed) .. " test suites passed ===")
if failed > 0 then os.exit(1) end
