require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
semantics = require './semanticAnalyser.coffee'
sys = require 'sys'
fs = require 'fs'
util = require 'util' # only for development and debugging
parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'
ffi = require 'node-ffi'

Node = ffi.Struct [
	["int32", "id"],
	["string", "type"],
	["string", "value"],
	["int32", "numberOfChildren"],
	["pointer", "children"]
]

parseTreeToC = (parseTree) ->
	syntaxTree = {} # prevents C structs from being garbage collected

	children = new ffi.Pointer 0
	children.attach syntaxTree
	if parseTree.numberOfChildren isnt 0
		children = new ffi.Pointer ( Node.__structInfo__.size * parseTree.numberOfChildren )
		children.putPointer (parseTreeToC node), true for node in parseTree.children 

		children = children.seek -(parseTree.numberOfChildren * 8)
		children.attach syntaxTree
		
	root = new Node
	root.id = parseTree.id
	root.type = parseTree.type
	root.value = parseTree.value
	root.numberOfChildren = parseTree.numberOfChildren
	root.children = children
	root.children.attach syntaxTree

	return root

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '

sys.puts source

ffiStruct = new ffi.Library "./libstruct", {
	"print_struct": [ "void", [ "pointer", "int32" ] ]
}

try
	parseTree = parser.parse source
	console.log (util.inspect parseTree, false, 50)
	#ffiStruct.print_struct (parseTreeToC parseTree).ref(), 0 
	labelTree = semantics.analyse parseTree
catch e
	if e.name is 'SemanticError'
		sys.puts e
	else if e.name is 'SyntaxError'
		sys.puts e.name + ": " + e.message + " at line " + e.line + ", col " + e.column
	else
		throw e
	process.exit 1
	
process.exit 0