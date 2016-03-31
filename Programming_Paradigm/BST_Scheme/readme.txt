The three main function that users can use in this program is the following 
(currently does not support duplicate numbers in the same list):

(bst ls)            ;takes in a list and outputs the binary search tree: T
(additionFun n T)   ;takes in a number n and a bst T and outputs a new T with n added to T
(deletionFUn n T)   ;takes in a number n and a bst T and outputs a new T with n deleted from T

Example:
Input:
(bst '(60 24 61 12 84 97))
(additionFun 1 (list (list '() 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '()))))
(deletionFun 1 (list (list '() 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '()))))
(additionFun 12 (list (list '() 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '()))))

Output:
(list (list '() 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '())))
(list (list (list '() 1 '()) 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '())))
(list (list '() 12 (list '() 24 '())) 60 (list (list '() 61 '()) 84 (list '() 97 '())))
Error: there is a duplicate number

Design Explanation:
(bst ls) steps:
    1. sort the the list ls
        -When I saw the example output, I realize that all binary search trees are sorted,
         so the first thing to do is to sort the list
    2. convert the list into bst form
        -Turning a sorted list into a binary search tree is tricky, I had to use another 
         function to help create the tree
    3. output the bst form
        -Output the tree at the end
(additionFun n T) steps:
    1. convert T into a list
        -first convert T into a list by using the bst_to_ls function
    2. add n to the list
        -add the new number in any order to the list
    3. apply bst to the new list
        -see (bst ls) steps
    4. output the bst form
        -output the new binary search tree with n added
(deletionFun n T) steps:
    1. convert T into a list
        -first convert T into a list by using the bst_to_ls function
    2. find n from the list and remove it
        -do a search by comparing the first number in the list to the rest of the 
         list and see if there's a match with n, if there is than form a new list
         without that number in the list, if n isn't found than form a new list that
         is exactly the same as the old list
    3. apply bst to the new list
        -see (bst ls) steps
    4. output the bst form
        -output the new binary search tree with n deleted

Program Summary:

============== Global variables =================


(define stack1 '())
    -stack1 is used to store the sorted list as it is being sorted
(define stack2 '())
    -stack2 is a helper stack to take all of stack1's numbers when the new number added to
     stack1 isn't following the sorting order
(define sorted_ls '())
    -sorted_ls is used to hold the final sorted list
(define bst_stack '())
    -bst_stack is used to store the binary search tree T
(define ls_stack '())
    -ls_stack is used to store the list after being converted from the binary search tree T
(define temp '())
    -temp is a temporary variable used to hold the list where a number is deleted from it's original list

================ Sort function and it's helpers ====================

(define (return x) x)   
    -this returns the value x from a function
(define (fill_stk1 dummy)
    -this function refills stack1 with all the values from stack2
(define (for_each_stk1 num)
    -this function takes in a number and compares it to the first number in stack1
     than decides to put it into stack1 or stack2
    -stack1 is the main stack that's use to store the completed or partial completed sort_ls
    -stack2 is used to allow reformatting stack1 by taking all the previous values from stack1
     when num does not follow it's order
(define (sort_ls ls)
    -this function sorts the list ls from smallest number to largest number
    
================= Converting list to Binary Search Tree function and it's helpers =================

(define (create_half_tree ls front end)
    -return a partial list with indexes from front to end of the list ls
(define (goto_num ls num)
    -takes in the index num and returns the value of that index from the list ls
(define (ls_to_bst ls)
    -converts a list (should be sorted) to the Binary Search Tree

================= User level functions of the Binary Search Tree and it's helpers =================

(define (go_thru ls n)
    -helper function for check_dup that goes through the ls one time
(define (check_dup ls)
    -checks for duplicate numbers in the list, if found return #t else #f
********(define (bst ls)********
    -the Binary Search Tree function that takes a list and turns it into a bst
(define (num_to_ls num) (cons num '()))
    -takes the number num and return it in a list
(define (bst_to_ls T)
    -converts the binary search tree to a list
*******(define (additionFun n T)*********
    -adds a number n to the binary tree T and return the new tree
(define (remove_num n ls)
    -remove the number n from the list and set global variable temp as the new list
*******(define (deletionFun n T)*********
    -delete the number n from the binary tree T and return a new binary tree