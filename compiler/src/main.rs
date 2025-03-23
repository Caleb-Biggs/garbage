use std::io::stdin;
use crate::lexeme::*;
pub mod lexeme;


macro_rules! Iter { ($T:ty) => (impl Iterator<Item=$T>); }


fn main() {
    let input = stdin().lines().map(|d| d.expect("STDIN Error"));
    let preprocessed = preprocessor(input);
    let tokens = tokenize(preprocessed);
    let _lexed = lexer(tokens);
}


fn preprocessor(input: Iter!(String)) -> Iter!(String) {
    return input;
}


fn tokenize(input: Iter!(String)) -> Iter!(String) {
    return input.flat_map(|line| {
        let mut tokens = Vec::<String>::new();
        let mut token = String::new();
        for c in line.chars() {
            // TODO: Use Rust's built in is_whitespace()?
            // TODO: Comments
            // TODO: Literals
            if c.is_whitespace() {
                if token.len() == 0 { continue; }
                tokens.push(token);
                token = String::new();
            } else { token.push(c); }
        }
        if token.len() > 0 { tokens.push(token); }
        return tokens;
    });
}


fn lexer(input: Iter!(String)) -> Iter!(Lexeme) {
    return input.map(|token| Lexeme::new(token));
}