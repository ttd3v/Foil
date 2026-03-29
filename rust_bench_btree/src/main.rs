use std::collections::BTreeMap;
use std::time::{Duration, Instant};

const TARGET: usize = 1_000_000;

fn main() {
    let mut rng: u64 = 0x123456789ABCDEF0;

    let mut map = BTreeMap::new();
    let start = Instant::now();

    let mut adds = 0u64;
    let mut removes = 0u64;
    let mut gets = 0u64;

    for _ in 0..TARGET {
        rng = rng.wrapping_mul(6364136223846793005).wrapping_add(1);
        let key = (rng & 0xFFFFFFFF) as u64;
        let op = ((rng >> 32) & 0x3) as u8;

        match op {
            0 | 3 => {
                map.insert(key, key);
                adds += 1;
            }
            1 => {
                map.remove(&key);
                removes += 1;
            }
            _ => {
                let _ = map.get(&key);
                gets += 1;
            }
        }
    }

    let elapsed = start.elapsed();
    let total_us = elapsed.as_micros() as u64;
    let ops_per_sec = if total_us > 0 {
        (TARGET as u64 * 1_000_000) / total_us
    } else {
        0
    };

    let net_ops = adds.saturating_sub(removes);
    let final_size = map.len() as u64;

    println!();
    println!("  ┌────────────────────────────────────────────────────────────┐");
    println!("  │         std::collections::BTreeMap Benchmark               │");
    println!("  └────────────────────────────────────────────────────────────┘");
    println!("  target {} ops", fmt_grouped(TARGET as u64));
    println!();
    println!("  operation                      time          throughput    ");
    println!("  ────────────────────────────────────────────────────────────");

    print!("  mixed ops            ");
    print!("{:>12} µs   ", fmt_grouped(total_us));
    print!("{:>15} ops/s", fmt_grouped(ops_per_sec));
    println!();

    println!("  ────────────────────────────────────────────────────────────");
    println!(
        "  total   {} µs   —   {} ops/s",
        fmt_grouped(total_us),
        fmt_grouped(ops_per_sec)
    );
    println!(
        "  final tree size: {} nodes  •  net ops: {} (adds - removes)",
        fmt_grouped(final_size),
        fmt_grouped(net_ops)
    );
    println!(
        "  adds: {}  removes: {}  gets: {}",
        fmt_grouped(adds),
        fmt_grouped(removes),
        fmt_grouped(gets)
    );
    println!();
}

fn fmt_grouped(n: u64) -> String {
    let s = n.to_string();
    let mut result = String::new();
    let len = s.len();
    for (i, c) in s.chars().enumerate() {
        if i > 0 && (len - i) % 3 == 0 {
            result.push(',');
        }
        result.push(c);
    }
    result
}

