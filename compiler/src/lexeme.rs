use std::collections::HashMap;
use std::sync::OnceLock;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;


#[derive(Clone)]
pub enum Lexeme {
    Identifier { name: String },
    Keyword { token: KeywordLexeme },
    Seperator { symbol: SeperatorLexeme },
    Operator { token: OperatorLexeme },
    Literal { value: LiteralLexeme },
}

#[derive(Clone, EnumIter)]
pub enum KeywordLexeme {
	// Used
	Break,
	Continue,
	Else,
	Enum,
	For,
	If,
	Return,
	Static,
	Struct,
	Match,
	While,
	Loop,
	Mut,
	Become,
	// Reserved
	Switch,
	Auto,
	Extern,
	Register,
	Case,
	Const,
	Default,
	Do,
	Goto,
	Sizeof,
	Typedef,
	Union,
 	Volatile,
}

#[derive(Clone, EnumIter)]
pub enum SeperatorLexeme {
	// Brackets
	ParenOpen,		// (
	ParenClose,		// )
	SquareOpen,		// [
	SquareClose,	// ]
	CurlyOpen,		// {
	CurlyClose,		// }
	// Other
	Semicolon,		// ;
	Colon,			// :
	Comma,			// ,
	Period,			// .
	// Reserved
	DoubleColon		// ::
}

#[derive(Clone, EnumIter)]
pub enum OperatorLexeme {
	// Arithmetic
	Addition,		// +
	Subtraction,	// -
	Multiplication,	// *
	Division,		// /
	Modulo,			// %
	// Comparison
	Equal,			// ==
	NotEqual,		// !=
	GreaterThan,	// >
	LessThan,		// <
	GreaterOrEqual,	// >=
	LessOrEqual,	// <=
	// Logical
	LogicalNot,		// !
	LogicalAnd,		// &&
	LogicalOr,		// ||
	// Bitwise
	BitNot,			// ~
	BitAnd,			// &
	BitOr,			// |
	BitXor,			// ^
	BitShiftLeft,	// <<
	BitShiftRight,	// >>
	// Assignment
	Assignment,		// =
	PlusEq,			// +=
	MinusEq,		// -=
	TimesEq,		// *=
	DivideEq,		// /=
	ModuloEq,		// %=
	AndEq,			// &=
	OrEq,			// |=
	XorEq,			// ^=
	LeftShiftEq,	// <<=
	RightShiftEq,	// >>=
	// Other
	Member,			// .
	// Reserved
	Increment,		// ++
	Decrement,		// --
	Address,		// &
	PointerMember,	// ->
	Ternary,		// ?
}

#[derive(Clone)]
pub enum LiteralLexeme {
	Placeholder { literal: String }
}

static LOOKUP: OnceLock<HashMap<String,Lexeme>> = OnceLock::new();

impl Lexeme {
	pub fn new(token: String) -> Lexeme {
		return match Self::lookup_token(&token) {
			Some(l) => l.clone(),
			// TODO: Literals
			None => Lexeme::Identifier { name: token }
		}
	}

	fn lookup_token(token: &String) -> Option<&Lexeme> {
		let lookup = LOOKUP.get_or_init(|| {
			let mut output = HashMap::<String,Lexeme>::new();
			let _ = KeywordLexeme::iter().map(|l| output.insert(l.string(), l.lex()));
			let _ = SeperatorLexeme::iter().map(|l| output.insert(l.string(), l.lex()));
			let _ = OperatorLexeme::iter().map(|l| output.insert(l.string(), l.lex()));
			return output;
		});
		return lookup.get(token);
	}
}


impl KeywordLexeme {
	fn string(&self) -> String {
		return "Placeholder".to_string();
	}

	fn lex(&self) -> Lexeme {
		return Lexeme::Keyword { token: self.clone() };
	}
}


impl SeperatorLexeme {
	fn string(&self) -> String {
		return "Placeholder".to_string();
	}

	fn lex(&self) -> Lexeme {
		return Lexeme::Seperator { symbol: self.clone() };
	}
}


impl OperatorLexeme {
	fn string(&self) -> String {
		return "Placeholder".to_string();
	}

	fn lex(&self) -> Lexeme {
		return Lexeme::Operator { token: self.clone() };
	}
}