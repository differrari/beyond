(load "~/redbuild/v3/redbuild.lisp")

(redbuild:set-global-target :redacted)

(defvar *beyond-lib* t)
(defvar *beyond-interpreter* t)

(redbuild:build-dep "/home/di/beyond")

(redbuild:quick-build (make-instance `redbuild:redmod
    :name "lfsp"
    :type :bin
    :target :redacted
    :libs (list (redbuild:local-lib "beyond" :lib "imaginal.a"))
    :srcs (list 
		"language.c" 
		"main.c"
	)
) :add-dependencies t :run nil :success 
    (lambda () 
        (uiop:run-program (list "cp" "lfsp.elf" "/home/di/os/fs/redos/services/lfsp") :output t :error-output t)
        (redbuild:make "~/os" "run" "-j")
    )
)