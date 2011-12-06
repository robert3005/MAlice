require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
semantics = require './semanticAnalyser.coffee'
sys = require 'sys'
fs = require 'fs'
util = require 'util' # only for development and debugging
parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '
sys.puts source
try
	parseTree = parser.parse source
	#labelTree = semantics.analyse parseTree
catch e
	if e.name is 'SemanticError'
		sys.puts e
	else if e.name is 'SyntaxError'
		sys.puts e.name + ": " + e.message + " at line " + e.line + ", col " + e.column
	process.exit 1
	
console.log (util.inspect parseTree, false, 50)
#sys.puts (semantics.buildtree parseTree).join ""

process.exit 0