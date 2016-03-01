sch.rb is a Scheme Interpreter written in ruby. It recognizes the following scheme expressions:
+,-,*,/,%,=,let,quote,if
The Interpreter works by consuming each token and immediately updates the token list, therefore it also works with compound scheme codes. The following is the class structure along with the methods of the Interpreter.

// this is a helper class with one method use to check if string is a number.
Class: String
// the numeric? method set's the calling string to a Float and returns true if it's a number or false otherwise
	def: numeric?
// this is the main class for the Scheme Interpreter
Class: SchemeInterpreter
	// initializes the string to it's tokenize array and creates a @@tokenize global variable for the class methods
	def: initialize
	// calculates all the arithmetic Scheme expressions and return it's results. Works for the following symbols: +,-,*,/,%
	def: arithmetic_check
	// stores varibles and calculates expression using the stored varibles. Works for the following form: (let ((variables)) expression )
	// currently does not support this form: (let function_name ((variables)) expression )
	def: let_check
		// the set_variable method sets all the ((varible)) into a Hash Map and returns the Map
		def: set_variable
	// calculates the boolean predicate expression and returns first atom if true and second atom if false
	// atoms can also be expressions
	def: if_check
		// sets the atom values from the predicate expression into a list for lambda comparison
		def: set_values
		// calculates the predicate and returns the boolean result
		def: bool_check
	// returns the quoted form of the Scheme input
	def: quote_check
		// converts the tokens into a quote string
		def: quote_convert
	// this method is call at every beginning of every Scheme expression. It directs the code to the different methods based on the operating symbol of each expression.
	def: root_check
	// executes the scheme code
	def: exe
	// displays the token list
	def: display_token
// this is a console input check to make sure the parenthesis match up before executing the scheme code
def: paren_check