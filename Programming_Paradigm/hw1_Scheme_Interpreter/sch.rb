=begin
Steven Ou
CS 152
2/14/2016
HW 1

Scheme Interpreter (sch.rb)
Recognize the following scheme expressions:
+,-,*,/,%,=,let,quote,if

+: adds the next two atoms together and return the result
-: subtracts the next two atoms together and return the result
*: multiplies the next two atoms together and return the result
/: divides the next two atoms together and return the result
=: used inside the condition part of if expression
quote: makes the next (expression) a string value
let: makes the next atom a variable that takes in the 
	next value
if: makes the next (expression) the condition that executes
	the expression after if true and the expression after this
	if it's false
	(< a b)
	(<= a b)
	(> a b)
	(>= a b)
	(equal? a b)
#t if 'a' is exactly equal to 'b', #f otherwise.

- Prompt for reading file or console
- Read scheme line as 1 string
- arrange all elements into 1 list splitting each element by spaces
- method parenCheck: create parenthesis enclosing check
	- check first parenthesis (return error if not found)
	- check if next element is one of the above scheme expression
		(return error if it's not expression 
		 unless the current or next element is 
		 closing parenthesis)
	- check if next element is a recognized value
		(if it is open parenthesis, call parenCheck
		if it is value move on, if it is string
		return error unless it follows let or quote)
	- check last parenthesis (return error if not found)
	- method exec: execute after last parenthesis check
- method exec: execute code base on scheme syntax

The following scheme codes have been tested and confirmed:	
(- 10 5)
(- 10 5 6)
(* 2 3)
(* 2 3 1)
(quote (1 2 3))
(quote "ciao")

(* (+ 2 6) (- 5 1))
(/ (+ 10 2) (+ 2 3))
(let ((a 2)) (if (= a 2) 0 2))
=end

# numeric? method
# reference: http://mentalized.net/journal/2011/04/14/ruby-how-to-check-if-a-string-is-numeric/
class String
  def numeric?
    Float(self) != nil rescue false
  end
end

class SchemeInterpreter
	#initialize the list of tokens
	def initialize(string)
		@tokenize = string.gsub('(','( ').gsub(')',' )')
						  .gsub('[','[ ').gsub(']',' ]').split
		#declare class global variable		
		@@tokenize = @tokenize
	end
			
	#checks the arithmetics given first atom is an arithmetic symbol
	def arithmetic_check
		#error check
		if  @@tokenize[0] == nil ||
			@@tokenize[1] == nil ||
			@@tokenize[2] == nil ||
			@@tokenize[3] == nil
			abort("Illegal input: #{@@tokenize[0]}") 
		end
		#form: (op a b) 
		op = @@tokenize[0]
		@@tokenize = @@tokenize[1..@@tokenize.length]

		#lambda expression for each arithmetic symbol
		sum = lambda { |a| 
			sum = a[0]
			i = 1
			while i < a.length 
				sum = sum + a[i]
				i = i + 1
			end
			return sum 
		}
		sub = lambda { |a| 
			sub = a[0]
			i = 1
			while i < a.length 
				sub = sub - a[i]
				i = i + 1
			end
			return sub 
		}
		mul = lambda { |a| 
			mul = a[0]
			i = 1
			while i < a.length
				mul = mul * a[i]
				i = i + 1
			end
			return mul 
		}
		div = lambda { |a| 
			div = a[0]
			i = 1
			while i < a.length
				div = div / a[i]
				i = i + 1
			end
			return div 
		}
		mod = lambda { |a| a[0] % a[1] }

		#define a consume a, define b consume b
		#puts all numerical values into numbers array
		i = 0
		numbers = []
		#puts @@tokenize[2]
		while @@tokenize[2] != ')'
			if @@tokenize[0] == '('
				numbers[i] = root_check
			elsif @@tokenize[0].numeric?
				numbers[i] = @@tokenize[0].to_f
				@@tokenize = @@tokenize[1..@@tokenize.length]
			else
				break
			end
			i = i + 1
		end
		if @@tokenize[0].numeric?
			numbers[i] = @@tokenize[0].to_f
			@@tokenize = @@tokenize[1..@@tokenize.length]
			i = i + 1
		end
		if @@tokenize[0].numeric?
			numbers[i] = @@tokenize[0].to_f
			@@tokenize = @@tokenize[1..@@tokenize.length]
			i = i + 1
		end

		#calculates the solution by calling lambda functions
		if  op == '+'
			solution = sum.call(numbers)
		elsif op == '-'
			solution = sub.call(numbers)
		elsif op == '*'
			solution = mul.call(numbers)
		elsif op == '/'
			solution = div.call(numbers)
		elsif op == 'modulo'
			if numbers.length > 2
				abort("Illegal input: #{@@tokenize[0]}") 
			else
				solution = mod.call(numbers)
			end
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end
		
		#return result when the end is reached
		if @@tokenize[0] == ')'
			@@tokenize = @@tokenize[1..@@tokenize.length]
			return solution
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end
	end

	def let_check
		#form (let function_name ((variables)) expression ) or
		#     (let ((variables)) expression )
		#error check
		if  @@tokenize[0] == nil ||
			@@tokenize[1] == nil ||
			@@tokenize[2] == nil ||
			@@tokenize[3] == nil ||
			@@tokenize[4] == nil ||
			@@tokenize[5] == nil ||
			@@tokenize[6] == nil 
			abort("Illegal input: #{@@tokenize[0]}") 
		end

		def set_variable(variable_map)
			variable_map[@@tokenize[0]] = @@tokenize[1]
			@@tokenize = @@tokenize[2..@@tokenize.length]
		
			if  @@tokenize[0] == ')' ||
				@@tokenize[0] == ']' ||
				@@tokenize[0] == '}'
				@@tokenize = @@tokenize[1..@@tokenize.length]
				return variable_map
			else
				abort("Illegal input: #{@@tokenize[0]}") 
			end
		end

		variable_map = Hash.new
		@@tokenize = @@tokenize[2..@@tokenize.length]

		#set variables into varible hash
		while @@tokenize[0] != ')'
			if @@tokenize[0] == '(' ||
			   @@tokenize[0] == '[' ||
			   @@tokenize[0] == '{' 
			    @@tokenize = @@tokenize[1..@@tokenize.length]
				variable_map = set_variable(variable_map)
			else
				
				abort("Illegal input: #{@@tokenize[0]}") 
			end
		end

		if  @@tokenize[0] == ')' ||
			@@tokenize[0] == ']' ||
			@@tokenize[0] == '}' 
			@@tokenize = @@tokenize[1..@@tokenize.length]
		end

		solution = 0
		#loops through the rest of @@tokenize and convert all varibles to numbers
		variable_map.each { |key,value| 
			i = 0
			while i < @@tokenize.length
				if @@tokenize[i] == key
					@@tokenize[i] = value
				end
				i = i + 1
			end
		}

		if @@tokenize[0] == "("
			solution = root_check
		elsif @@tokenize[0].numeric?
			solution = @@tokenize[0]
			@@tokenize = @@tokenize[1..@@tokenize.length]
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end

		if @@tokenize[0] == ')'
			@@tokenize = @@tokenize[1..@@tokenize.length]
			return solution
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end
	end

	#checks the if statement and return result
	def if_check 
		#error check
		if  @@tokenize[0] == nil ||
			@@tokenize[1] == nil ||
			@@tokenize[2] == nil ||
			@@tokenize[3] == nil ||
			@@tokenize[4] == nil
			abort("Illegal input: #{@@tokenize[0]}") 
		end

		#sets numerical values into an array and returns the array
		def set_values(a)
			if @@tokenize[0].numeric?
				a[0] = @@tokenize[0].to_f
				@@tokenize = @@tokenize[1..@@tokenize.length]
			elsif @@tokenize[0] == '('
				a[0] = root_check
			else
				abort("Illegal input: #{@@tokenize[0]}") 
			end
			if @@tokenize[0].numeric?
				a[1] = @@tokenize[0].to_f
				@@tokenize = @@tokenize[1..@@tokenize.length]
			elsif @@tokenize[0] == '('
				a[1] = root_check
			else
				abort("Illegal input: #{@@tokenize[0]}") 
			end
			return a
		end

		#checks the bool value and return true or false
		def bool_check
			les_th = lambda {|a, b| a < b  ? true : false}
			les_eq = lambda {|a, b| a <= b ? true : false}
			gre_th = lambda {|a, b| a > b  ? true : false}
			gre_eq = lambda {|a, b| a >= b ? true : false}
			_equal = lambda {|a, b| a == b ? true : false}

			a = []
			bool_result = true
			if @@tokenize[0] != '('
				if  @@tokenize[0] == 'true' ||
					@@tokenize[0] == '#t'   ||
					@@tokenize[0] == '\'#t' 
					return true
				elsif @@tokenize[0] == 'false' ||
					  @@tokenize[0] == '#f'    ||
					  @@tokenize[0] == '\'#f' 
					return false
				else
					abort("Illegal input: #{@@tokenize[0]}") 
				end
			else
				@@tokenize = @@tokenize[1..@@tokenize.length]
				if    @@tokenize[0] == '<'
					@@tokenize = @@tokenize[1..@@tokenize.length]
					a = set_values(a)
					bool_result = les_th.call(a[0],a[1])
				elsif @@tokenize[0] == '<='
					@@tokenize = @@tokenize[1..@@tokenize.length]
					a = set_values(a)
					bool_result = les_eq.call(a[0],a[1])
				elsif @@tokenize[0] == '>'
					@@tokenize = @@tokenize[1..@@tokenize.length]
					a = set_values(a)
					bool_result = gre_th.call(a[0],a[1])
				elsif @@tokenize[0] == '>='
					@@tokenize = @@tokenize[1..@@tokenize.length]
					a = set_values(a)
					bool_result = gre_eq.call(a[0],a[1])
				elsif @@tokenize[0] == '='    ||
					  @@tokenize[0] == 'eq?'  ||
					  @@tokenize[0] == 'eqv?' ||
					  @@tokenize[0] == 'equal?'
					@@tokenize = @@tokenize[1..@@tokenize.length]
					a = set_values(a)
					bool_result = _equal.call(a[0],a[1])
				else
					abort("Illegal input: #{@@tokenize[0]}") 
				end
			end
			if @@tokenize[0] == ')'
				@@tokenize = @@tokenize[1..@@tokenize.length]
				return bool_result
			else
				abort("Illegal input: #{@@tokenize[0]}") 
			end
		end

		#find predicate
		@@tokenize = @@tokenize[1..@@tokenize.length]
		array = []
		_predicate = bool_check
		return_exp = set_values(array)
		_predicate ? solution = return_exp[0] : solution = return_exp[1]

		#returns result when the end is reached
		if @@tokenize[0] == ')'
			@@tokenize = @@tokenize[1..@@tokenize.length]
			return solution
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end
	end

	#return the scheme expression in quote form
	def quote_check
		# error check
		if  @@tokenize[0] == nil ||
			@@tokenize[1] == nil ||
			@@tokenize[2] == nil
			abort("Illegal input: #{@@tokenize[0]}1") 
		end

		# converts the token string into quoted form
		def quote_convert(string)
			converted_string = ""
			# error check
			if string == '#'
				abort("Illegal input: #{@@tokenize[0]}2") 
			end
			if  string.numeric?   || 
			 	string[0] == "\'" ||
			 	string[0] == '\"'
				converted_string = string + " "
			elsif string == '(' ||
				  string == '{' ||
				  string == '['
				converted_string = '(list '
			elsif string == ')' ||
				  string == '}' ||
				  string == ']' 
				converted_string = ') '
			else
				converted_string = "\'" + string + " "
			end
			return converted_string
		end

		quote_string = ""
		@@tokenize = @@tokenize[1..@@tokenize.length]

		if @@tokenize[0] != '(' && @@tokenize[1] == ')'
			quote_string = quote_convert(@@tokenize[0])
			@@tokenize = @@tokenize[1..@@tokenize.length]
		elsif @@tokenize[0] == '('
			i = 0
			while i < @@tokenize.length-1
				if  @@tokenize[i] == ')' ||
					@@tokenize[i] == '}' ||
					@@tokenize[i] == ']'
					quote_string = quote_string[0..quote_string.length-2] +
									quote_convert(@@tokenize[i])
				else
					quote_string = quote_string + quote_convert(@@tokenize[i])
				end
				i = i + 1
			end
			@@tokenize = @@tokenize[i..@@tokenize.length]
 		else 
			abort("Illegal input: #{@@tokenize[0]}3") 
		end

		#returns the quoted string when the end is reached
		if @@tokenize[0] == ')'
			@@tokenize = @@tokenize[1..@@tokenize.length]
			return quote_string
		else
			abort("Illegal input: #{@@tokenize[0]}4") 
		end
	end

	#checks the beginning of a new expression and send it to the corresponding methods
	def root_check
		#error checking
		if @@tokenize[0] != '('
			abort("Illegal input: #{@@tokenize[0]}") 
		end

		#op checking
		@@tokenize = @@tokenize[1..@@tokenize.length]
		if  @@tokenize[0] == '+' ||
			@@tokenize[0] == '-' ||
			@@tokenize[0] == '*' ||
			@@tokenize[0] == '/' ||
			@@tokenize[0] == 'modulo' 
			return arithmetic_check
		elsif @@tokenize[0] == 'let'
			return let_check
		elsif @@tokenize[0] == 'if'
			return if_check
		elsif @@tokenize[0] == 'quote'
			return quote_check
		else
			abort("Illegal input: #{@@tokenize[0]}") 
		end
	end
	
	#execute tokenize and return result
	def exe
		if @@tokenize[0].numeric? 
			return @@tokenize[0]
		elsif @@tokenize[0] != '('
			abort("Illegal input: #{@@tokenize[0]}") 
		end
		return root_check
	end

	def display_token
		print @tokenize
	end
end

#checks that the parenthesis match from the input
def paren_check(string)
	i, left_count, right_count = 0,0,0
  	while i < string.length
	  	if string[i] == '(' 
	  		left_count = left_count + 1
	  	elsif string[i] == ')'
	  		right_count = right_count + 1
	  	end
  		i = i + 1
	end
	if left_count > 0 && right_count > 0 && left_count == right_count
  		return true
  	else 
  		return false
  	end
end

#main method begins
prompt = true
while prompt
	print "Enter Scheme code: "
	string = gets.chomp + " "
	if string.length == 1
		scheme = SchemeInterpreter.new(string)
		puts scheme.exe
	else
		while !paren_check(string)
			string = string + gets.chomp + " "
		end
		scheme = SchemeInterpreter.new(string)
		puts scheme.exe
	end

	print "Would you like to enter another Scheme code (y/n)? "
	if gets.chomp == 'y'
		prompt = true
	else
		prompt = false
	end
end
abort "End Program!"