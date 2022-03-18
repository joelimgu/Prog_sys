
use std::thread;
use array_macro::array;
use rand::prelude::*;
use std::time::Instant;

const D: f64 = 5.0;
const NB_THREADS: u32 = 1000;
const NB_ARROWS: u32 = NB_THREADS*1000000;

fn random_float(min: f64, max: f64) -> f64 {
    rand::thread_rng().gen_range(min..=max)
}

fn is_point_in_circle(x: f64, y:f64) -> bool {
    (x*x + y*y) < (D/2.0)*(D/2.0)
}

fn throw_arrows(nb_of_arrows: u32) -> i32 {
    let mut x: f64;
    let mut y: f64;
    let mut res: i32 = 0;
    let mut result: Vec<bool> = Vec::with_capacity(nb_of_arrows as usize);

    let _ = Instant::now();

    for _ in 0..nb_of_arrows {
        x = random_float(-D/2.0,D/2.0);
        y = random_float(-D/2.0,D/2.0);
        result.push( is_point_in_circle(x,y));
    }
    for i in result {
        res += i as i32;
    }
    res
}

fn main() {
    let arrows_per_thread = NB_ARROWS/NB_THREADS;
    let mut results = [0; NB_THREADS as usize];
    let threads= array![ _ => {
        thread::spawn(move || {
        throw_arrows(arrows_per_thread)
    })}; NB_THREADS as usize];
    let now = Instant::now();

    let mut i = 0;
    for h in threads {
        results[i as usize] = h.join().unwrap();
        i += 1;
    }
    let fc = results.into_iter()
        .reduce(|a,b| a + b)
        .unwrap();

    let pi = 4.0*(fc as f64)/NB_ARROWS as f64;
    let elapsed = now.elapsed();

    println!("PI = {pi}");
    println!("Took {:.4?}",elapsed);
}
