#!/bin/sbcl --script
(load "~/redbuild/v3/redbuild.lisp")

(defvar *beyond-lib* nil)
(defvar *beyond-lang* "cred")
(defvar *beyond-interpreter* nil);(eq *beyond-lang* "lisp"))

(defmacro exname () (if (eq *beyond-interpreter* t) "imaginal" "cred"))
(defmacro extype () (if (eq *beyond-lib* t) :lib :bin))
(defmacro if-comp (a b) (if (eq *beyond-interpreter* nil) a b))

(redbuild:quick-build (make-instance `redbuild:redmod
    :name "rulegen"
    :type :bin
    :target (redbuild:native)
    :srcs (list 
		"ir/manual_gen.c" 
		"codegen/codeformat.c" 
		"codegen/codegen.c" 
		"codegen/identity_transform.c"
		"ir/general.c" 
		"semantic/sem_analysis.c"
		"semantic/semantic_rules.c"
		"rule_generator/rulecodegen.c" 
		"rule_generator/ruleparser.c" 
		"rule_generator/rule_transform.c"
	)
    :flags (list 
        "-DNOCODETRANSFORM"
        "-DRULECODEGEN"
        "-DRULETRANSFORM"
    )
) :add-dependencies t :run t :run-args (if-comp (format nil "languages/~a.rules" *beyond-lang*) "languages/lisp.rules") :success (lambda () 
    (redbuild:quick-cred "semantic/sem_enum.cred" "semantic/semantic_rules")
    ; (redbuild:quick-cred "codegen/codegen.cred" "codegen/codegen");TODO: reenable this
    (redbuild:quick-build (make-instance `redbuild:redmod
        :name (exname)
        :type (extype)
        :target (redbuild:dyn-target)
        :srcs (redbuild:all-sources-ignoring "c" (redbuild:flatten 
            (list "output.c" "build.c" 
                (if-comp "imaginal.c" "cred_main.c") 
                ; (if-comp (redbuild:all-sources-in-dir (append (uiop:getcwd) "interpreter")) ".c") nil) 
                (if-comp "interpreter/repl.c" nil)
                (if-comp "interpreter/imaginal.c" nil)
                (if-comp "interpreter/debug_print.c" nil)
                "rule_generator/ruleparser.c"
                ; (if-comp (redbuild:all-sources-in-dir (append (uiop:getcwd) "rule_generator")) ".c") nil) 
                ;(redbuild:all-sources-in-dir "lfsp" "c")
                "lfsp/language.c"
                "lfsp/main.c"
                "ruleparser.c" "preproc_c.c"
            )))
        :flags (list 
            "-DCCODEGEN"
            "-DCTRANS"
            "-DNORULETRANSFORM"
        )
    ) :add-dependencies t :run t :run-args (if-comp "street.cred" "test.lisp") :success (lambda () (redbuild:emit-compile-commands)))
))
