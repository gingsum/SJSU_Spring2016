;these are global variables
(define stack1 '())
(define stack2 '())
(define sorted_ls '())
(define bst_stack '())
(define ls_stack '())
(define temp '())

(define (return x) x)   ;this returns the value x from a function
;this function refills stack1 with all the values from stack2
(define (fill_stk1 dummy)
  (unless (null? stack2)
      (begin
        (set! stack1 (cons (car stack2) stack1))
        (set! stack2 (cdr stack2))
        (fill_stk1 dummy)
      )
  )
)
;this function takes in a number and compares it to the first number in stack1
;than decides to put in stack1 or stack2
;stack1 is the main stack that's use to store the completed or partial completed sort_ls
;stack2 is used to allow reformatting stack1 by taking all the previous values from stack1
; when num does not follow it's order
(define (for_each_stk1 num)
  (begin
    (if (not (null? stack1))
      (cond ((> (car stack1) num)
        (begin
          (set! stack1 (cons num stack1))
          (unless (null? stack2)
            (fill_stk1 num)
          )
        ))
        ((< (car stack1) num)
        (begin
          (set! stack2 (cons (car stack1) stack2))
          (set! stack1 (cdr stack1))
          (for_each_stk1 num)
        ))
      )
      (set! stack1 (cons num stack1))
    )
    (unless (null? stack2)
          (fill_stk1 num)
    )
 )
)
;this function sorts the list ls from smallest number to largest number
(define (sort_ls ls)
  (if (not (null? ls))
    (begin
      (if (not (null? stack1))
        (for_each_stk1 (car ls))
        (set! stack1 (cons (car ls) stack1))
      )
      (sort_ls (cdr ls))
    )
    (begin
      (return stack1)
      (set! sorted_ls stack1)
      (return stack1)
    )
  )
)
;==================================================
;return a partial list with indexes from front to end of the list ls
(define (create_half_tree ls front end)
  (cond ((< front 1) (begin (display "front is out of bound") (void)))
        ((> end (length ls)) (begin (display "end is out of bound") (void)))
        ((> end front) (cons (goto_num ls front) (create_half_tree ls (+ front 1) end)))
        ((= end front) (cons (goto_num ls end) '()))
        (else '())
  )
)
;takes in the index num and returns the value of that index from the list ls
(define (goto_num ls num)
  (cond ((= num 0) '())
        ((= num 1) (car ls))
        ((> num (length ls)) "out of bound")
        (else (goto_num (cdr ls) (- num 1)))
  )
)
;converts a list (should be sorted) to the Binary Search Tree
(define (ls_to_bst ls)
  (cond ((= (length ls) 0) '())
        (else 
          (let ((current_index (+ (floor (/ (length ls) 2)) (modulo (length ls) 2)))
                (front 1)
                (end (length ls))
               )
               (begin
                  (append (append
                        (list (ls_to_bst (create_half_tree ls front (- current_index 1))))  ; left tree
                                (cons (goto_num ls current_index) '()))                     ; current node
                          (list (ls_to_bst (create_half_tree ls (+ current_index 1) end)))) ; right tree
                )
          )
        )
  )
)
;====================================================
;helper function for check_dup that goes through the ls one time
(define (go_thru ls n)
   (if (null? ls)
       #f
    (let ((num (car ls)))
         (begin
          (if (= num n)
              #t
              (go_thru (cdr ls) n)
          )
         )
    )
   )
)
;checks for duplicate numbers in the list, if found return #t else #f
(define (check_dup ls)
  (if (null? ls) 
      #f
      (begin
        (let ((num (car ls)))
          (if (go_thru (cdr ls) num) 
              #t
              (check_dup (cdr ls))
          )
        )
      )
  )
)

;the Binary Search Tree function that takes a list and turns it into a bst
(define (bst ls) 
  (cond ((null? ls) '())
        ((check_dup ls) "these is a duplicate number")     
        (else 
          (begin
            (set! stack1 '())
            (set! stack2 '())
            (set! bst_stack (ls_to_bst (sort_ls ls)))
            (return bst_stack)
          )
        )
  )
)
;takes the number num and return it in a list
(define (num_to_ls num) (cons num '()))
;converts the binary search tree to a list
(define (bst_to_ls T)
  (if (list? T)
      (let ((current_symbol (car T))
            (future_symbol (cdr T)))
        (begin   ;recursivily goes into the tree from left to right until it hits a null list or a number
           (cond ((null? current_symbol)
                  (cond ((null? future_symbol)
                         (void))
                        ((number? future_symbol)
                         (set! ls_stack (append ls_stack (num_to_ls future_symbol))))
                        ((list? future_symbol)
                         (bst_to_ls future_symbol))
                  ) 
                 )
                 ((number? current_symbol)
                  (begin
                    (set! ls_stack (append ls_stack (num_to_ls current_symbol)))
                    (cond ((null? future_symbol)
                           (void))
                          ((number? future_symbol)
                           (set! ls_stack (append ls_stack (num_to_ls future_symbol))))
                          ((list? future_symbol)
                           (bst_to_ls future_symbol))
                    )
                  )
                 )
                 ((list? current_symbol)
                   (begin
                     (bst_to_ls current_symbol)
                     (cond ((null? future_symbol)
                            (void))
                           ((number? future_symbol)
                            (set! ls_stack (append ls_stack (num_to_ls future_symbol))))
                           ((list? future_symbol)
                            (bst_to_ls future_symbol))
                     )    
                   )
                 )
           )
         )
      )
      (display "input must be a list")
   )
)
;adds a number n to the binary tree T and return the new tree
(define (additionFun n T) 
  (if (null? T) '()
    (begin
      (set! ls_stack '())
      (bst_to_ls T)
      (set! ls_stack (cons n ls_stack))
      (if (check_dup ls_stack)
          (display "there is a duplicate number")
          (bst ls_stack)
      )
    )
  )
)
;remove the number n from the list and set global variable temp as the new list
(define (remove_num n ls)
  (cond ((null? ls)(void))
        ((= n (car ls)) (remove_num n (cdr ls)))
        (else
         (begin
           (set! temp (append temp (list (car ls))))
           (remove_num n (cdr ls))
          )
        )
  ) 
)
;delete the number n from the binary tree T and return a new binary tree
(define (deletionFun n T)
  (if (null? T) '()
    (begin
      (set! ls_stack '())
      (set! temp '())
      (bst_to_ls T)
      (if (check_dup ls_stack) "there is a duplicate number"
        (begin
          (remove_num n ls_stack)
          (bst temp)
        )
      )
    )
  )
)