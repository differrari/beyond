(load "~/redbuild/v3/redbuild.lisp")

(let ((rule-mod (make-instance `redbuild:redmod
			       :name "rulegen"
			       :type :bin
			       :target :linux
			       :srcs (list 
										"ir/manual_gen.c" 
										"codegen/codeformat.c" 
										"codegen/identity_transform.c" 
										"codegen/codegen.c" "ir/general.c" 
										"rule_generator/rulecodegen.c" 
										"rule_generator/ruleparser.c" 
										"rule_generator/rule_transform.c"
						))))
  (redbuild:quick-build rule-mod :add-dependencies t :run t :success (lambda () (print "Done")))
  )

