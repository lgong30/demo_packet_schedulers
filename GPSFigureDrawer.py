# Load the jinja library's namespace into the current module.
import jinja2
import json
import os, os.path
import argparse
from sys import platform as _platform

if _platform == "linux" or _platform == "linux2":
   # linux
   clean = 'rm'
   pathsplit = '/'
   
elif _platform == "darwin":
   # MAC OS X
   clean = 'rm'
   pathsplit = '/'
   
elif _platform == "win32":
   # Windows
   clean = 'del'
   pathsplit = '\\'

   

# Command Line Description
parser = argparse.ArgumentParser(description='usage of GPS figure drawer (GPSFD)')
parser.add_argument('-t','--template',help='template filename (if there is only one tex file in your search path, you can choose to omit this input parameter, otherwise, you have to specify it)',type=str)
parser.add_argument('-s','--searchpath',help='the absolute path where templates can be found, omitting this input parameter indicating that the search path is the current directory',type=str)
parser.add_argument('-d','--data',help='input variables to the template as a json file (if there is only one tex file in your search path, you can choose to omit this input parameter, otherwise, you have to specify it)',type=str)
parser.add_argument('-o','--output',help='the filename template for the output PDFs, omitting it means you want to the template filename as the prefix of the PDF filenames',type=str)
args = parser.parse_args()

# In this case, we will load templates off the filesystem.
# This means we must construct a FileSystemLoader object.
# 
# The search path can be used to make finding templates by
#   relative paths much easier.  In this case, we are using
#   absolute paths and thus set it to the current folder.

if args.searchpath:
	searchPath = args.searchpath
	templateLoader = jinja2.FileSystemLoader(searchpath=args.searchpath)
else:
	searchPath = os.getcwd()
	templateLoader = jinja2.FileSystemLoader(searchpath=".")



# An environment provides the data necessary to read and
#   parse our templates.  We pass in the loader object here.
templateEnv = jinja2.Environment(loader=templateLoader)

# This constant string specifies the template file we will use.
if args.template:
	TEMPLATE_FILE = args.template
else:
	counter = 0

	for f in os.listdir(searchPath):
		if f.endswith('.tex'):
			counter += 1
			
			TEMPLATE_FILE = f
			if counter > 1:
				print 'Error: there are more than one TEX files in the path [%s], therefore, you have to specify which one is the template file' % searchPath
				exit(1)


# Read the template file using the environment object.
# This also constructs our Template object.
template = templateEnv.get_template(TEMPLATE_FILE)

# Specify any input variables to the template as a dictionary.
if args.data:
	inputVars = searchPath + '/' + args.data
else:
	counter = 0

	for f in os.listdir(searchPath):
		if f.endswith('.json'):
			counter += 1
			inputVars = searchPath + pathsplit + f
			if counter > 1:
				print 'Error: there are more than one JSON files in the path [%s], therefore, you have to specify which one is the data file' % searchPath
				exit(1)	

with open(inputVars) as f:
	#print f.readlines()
	data = json.loads(f.read())

# output directory
outputDir = searchPath

# output filename template 
if args.output:
	outputFilePrefix = args.output
else:
	outputFilePrefix = os.path.splitext(TEMPLATE_FILE)[0]

# parse the template, generating PDFs

# Finally, process the template to produce our final text.

sumWeights = 0
maxWeight = 0
flowWeights = copy.deepcopy(data["flowWeights"])
packets = copy.deepcopy(data["packets"])
flowNumber = len(flowWeights)
for fw in flowWeights:
	sumWeights += fw
	if maxWeight < fw:
		maxWeight = fw
maxVFTime = 0
for packet in packets:
	if packet["virtualFinishTime"] > maxVFTime:
		maxVFTime = packet["virtualFinishTime"]
	packet["weight"] = flowWeights[packet["flowId"] - 1]

X = flowNumber + 3
Y = maxVFTime + 3
DEF_FLOW_WIDTH = 0.8

for fw in flowWeights:
	fw = DEF_FLOW_WIDTH * fw / maxWeight

for packet in packets:
	packet["width"] = flowWeights[packet["flowId"] - 1]

templateVars = {
	"WIDTH": X,
	"HEIGHT": Y,
	"flowWeights": flowWeights,
	"packets": packets
}

outputText = template.render(templateVars)

output = outputDir + pathsplit + outputFilePrefix + '_' + comp["id"] + ".tex"
with open(output,"w") as f:
	f.write(outputText)

print outputSubDir
os.system('pdflatex -synctex=1 -interaction=nonstopmode -output-directory ' + outputSubDir + ' ' + output)

rExts = ['*.dvi', '*.out', '*.log', '*.aux', '*.ps', '*.idx', '*.lof', '*.ind', '*.blg', '*.toc', '*.ilg', '*~', '*.synctex']

for ext in rExts:
	print clean + ' ' + outputSubDir + pathsplit + ext
	os.system(clean + ' ' + outputSubDir + pathsplit + ext)

print '%s was generated successfully' % (outputSubDir + pathsplit + outputFilePrefix + '_' + comp["id"] + '.pdf')


	



