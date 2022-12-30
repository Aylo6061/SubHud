import sys
import csv

#get location of csv - it should be passed in as the first arg

sourceFileName = sys.argv[1]
outputFileName = sys.argv[2]

#open csv - its not big so just stick it all in a list

reader = csv.reader(open(sourceFileName,'r'),delimiter=',')
file = list(reader)

#get all states from first column
#get all the event types
states = []
events = []
stateDescriptions = []
onEntryFunctions = []
onExitFunctions = []

for num, row in enumerate(file):
	if(num != 0):
		states.append(row[0])
		stateDescriptions.append([1])
		onEntryFunctions.append(row[2])
		onExitFunctions.append(row[3])

	else:
		events=row[4:]
print("data\n%s\n"%file)
print("states: %s"%states)
print("events: %s"%events)
print("EntryFs: %s"%onEntryFunctions)
print("ExitFs: %s"%onExitFunctions)

#make a new output file

with open(outputFileName, 'w') as output:
	output.write("#include <arduino.h>\n")

#write state type declaration
	output.write("typedef enum\n{\n")
	for num, state in enumerate(states):
		output.write("%s = %s,\n"%(state, num))
	output.write("} state_t;\n\n")

#write event type declaration
	output.write("typedef enum\n{\n")
	for num, event in enumerate(events):
		output.write("%s = %s,\n"%(event,num))
	output.write("} event_t;\n\n")

#write the state table

	output.write("state_t stateTable [%s][%s] =\n{\n"%((len(states),len(events))))
	for num, state in enumerate(states):
		verticalHeaderOffset= 1 #vertical offset from header of csv
		horizontalHeaderOffset= 4 #horizontal offset from header of csv
		nextStateList = file[num+verticalHeaderOffset][horizontalHeaderOffset:]

		output.write("{")
		for item in nextStateList:
			if item in states:
				output.write(item+",")
			elif item == "":
				output.write(state+",")
			else:
				raise Exception("state machine transition points at a nonexistient state. state row %s, nonexistient state %s"%(state,item))
		output.write("},\n")
	output.write("};\n\n")

	output.write("""state_t currentState = modeWake;

bool isStateNew = 1;

	void stateMachineUpdate(event_t event)
	{
		state_t prevState;
		prevState = currentState;
		currentState = stateTable[currentState][event];

		if(currentState!= prevState){
			isStateNew = 1;
		}

	}

	bool newState(){
		if(isStateNew){
			isStateNew = 0;
			return 1;
		}
		else{
			return 0;
		}
	}""")

print("printed state machine file")