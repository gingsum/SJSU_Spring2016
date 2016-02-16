# Steven Ou
# 2/6/2016
# CS 160 Sec 1
# Assignment #1

# variable declaration
list = []
grand_count, 
dept_count, 
plant_count, 
state_count  = 0,0,0,0

# file input
File.foreach("widgets.csv") do |line|
	list = list + [line.gsub("\n","").split(",")]
end

# methods
def printDept(array, dept_count)
	printf("%30s", dept_count.to_s + " ")
	print "TOTAL FOR DEPT".ljust(15)
	printf("%3s", array[2]) 
	print " *\n" 
end

def printPlant(array, plant_count)
	printf("%30s", plant_count.to_s + " ")
	print "TOTAL FOR PLANT".ljust(15)
	printf("%3s", array[1]) 
	print " **\n" 
end

def printState(array, state_count)
	printf("%30s", state_count.to_s + " ")
	print "TOTAL FOR STATE".ljust(15)
	printf("%3s", array[0]) 
	print " ***\n" 
end

def printGrand(grand_count)
	printf("%30s", grand_count.to_s + " ")
	print "GRAND TOTAL".ljust(15)
	printf("%3s", "") 
	print " ****" 
end

def printDefault(array)
	printf("%6s", array[0])
	printf("%6s", array[1])
	printf("%5s", array[2])
	printf("%6s", array[3])
	printf("%7s", array[5] + " ")
	puts array[4]
end

# main function
printDefault(list[0])
puts ""
i = 1
while i < list.length
	#add count to State, Plant, Dept, and Grand total
	grand_count = grand_count + list[i][5].to_i
	state_count = state_count + list[i][5].to_i
	plant_count = plant_count + list[i][5].to_i
	dept_count = dept_count + list[i][5].to_i

	#printDefault
	printDefault(list[i])
	#check State and print
	if i+1 == list.length || (list[i][0] != list[i+1][0])
		puts ""
		printDept(list[i],dept_count)
		dept_count = 0
		printPlant(list[i], plant_count)
		plant_count = 0
		printState(list[i], state_count)
		state_count = 0
		puts ""
	#check Plant and print
	elsif list[i][1] != list[i+1][1]
		puts ""
		printDept(list[i], dept_count)
		dept_count = 0
		printPlant(list[i], plant_count)
		plant_count = 0
		puts ""
	#check Dept and print
	elsif list[i][2] != list[i+1][2]
		puts ""
		printDept(list[i], dept_count)
		dept_count = 0
		puts ""
	end
	i = i + 1
end
printGrand(grand_count)