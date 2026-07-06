(load "~/redbuild/v3/redbuild.lisp")

(redbuild:quick-build (make-instance `redbuild:redmod
    :name "rulegen"
    :type :bin
    :target (redbuild:native)
    :srcs (list 
		"ir/manual_gen.c" 
		"codegen/codeformat.c" 
		"codegen/codegen.c" 
		"ir/general.c" 
		"rule_generator/rulecodegen.c" 
		"rule_generator/ruleparser.c" 
		"rule_generator/rule_transform.c"
	)
    :flags (list 
        "-DNOCODETRANSFORM"
        "-DRULECODEGEN"
        "-DRULETRANSFORM"
    )
) :add-dependencies t :run t :run-args "languages/lisp.rules" :success (lambda () 
    (redbuild:quick-cred "semantic/sem_enum.cred" "semantic/semantic_rules")
    (redbuild:quick-cred "codegen/codegen.cred" "codegen/codegen")
    (redbuild:quick-build (make-instance `redbuild:redmod
        :name "cred"
        :type :bin
        :target (redbuild:native)
        :srcs (redbuild:all-sources-ignoring "c" (list "output.c" "build.c" "main.c" "ruleparser.c"))
        :flags (list 
            "-g"
            "-DCCODEGEN"
            "-DCTRANS"
            "-DNORULETRANSFORM"
        )
    ) :add-dependencies t :run t :run-args "test.lisp" :success (lambda () (redbuild:emit-compile-commands)))
))