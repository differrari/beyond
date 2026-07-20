import hardcoded_rules from './hardcoded.js';

export default grammar({
	name: "cred",
	extras: ($) => [
		// NEWLINE,
		/\s/,
		$.comment,
	],rules: {
		chain: $ => prec(100,choice(seq($.exp, optional($.chain)))),
		exp: $ => prec(99,choice(seq($.sexp),seq($.atom))),
		sexp: $ => prec(98,choice(seq($.LPAREN, optional($.args), $.RPAREN))),
		args: $ => prec(97,choice(seq($.exp, optional($.args)))),
		atom: $ => prec(96,choice(seq($.val),seq($.val),seq($.val))),
		val: $ => prec(95,choice(seq($.IDENTIFIER),seq($.STRING),seq($.NUMBER))),
		...hardcoded_rules
	}
});