use clap::Parser;

#[derive(Parser, Debug)]
#[command(name = "hello_world")]
#[command(about = "A simple hello world program with customizable greetings")]
struct Args {
    /// First name
    #[arg(short, long)]
    first_name: String,

    /// Last name
    #[arg(short, long)]
    last_name: String,

    /// Number of times to say hello
    #[arg(short, long, default_value = "1")]
    count: u32,
}

fn main() {
    let args = Args::parse();

    for _ in 0..args.count {
        println!("Hello, {} {}!", args.first_name, args.last_name);
    }
}
