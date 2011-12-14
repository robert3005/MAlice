require.paths.push '/usr/lib/node_modules'
semanticChecker = require './semanticAnalyser.coffee'
semantics = new semanticChecker
sys = require 'sys'
peg = require 'pegjs'
fs = require 'fs'
util = require 'util' # only for development and debugging
parser = peg.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'
ffi = require 'node-ffi'

Node = ffi.Struct [
	["int32", "id"],
	["string", "type"],
	["string", "value"],
	["pointer", "position"],
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

		children = children.seek -(parseTree.numberOfChildren * ffi.Bindings.POINTER_SIZE)
		children.attach syntaxTree
		
	root = new Node
	root.id = parseTree.id
	root.type = parseTree.type
	root.value = parseTree.value
	root.numberOfChildren = parseTree.numberOfChildren

	position = new ffi.Pointer ( 8 )
	position.putInt32 parseTree.position.column, true
	position.putInt32 parseTree.position.line, true
	position = position.seek -8

	root.position = position
	root.position.attach syntaxTree
	root.children = children
	root.children.attach syntaxTree

	return root

arguments = process.argv.splice 2

saveFileName = (source) ->
	pathTokens = arguments[0].split '/'
	fileNameTokens = pathTokens.pop().split '.'
	pathTokens.push fileNameTokens[0]
	pathTokens.join '/'

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '

#sys.puts source

codeGen = new ffi.Library "./libstruct", {
	"print_struct": [ "void", [ "pointer", "int32" ] ],
	"compile": [ "void", [ "pointer", "string" ] ] 
}

try
	parseTree = parser.parse source
	#sys.puts (util.inspect parseTree, false, 50)
	codeGen.compile (parseTreeToC parseTree).ref(), saveFileName arguments[0]
	#semantics.analyse parseTree
catch e
	if e.name is 'SemanticError'
		sys.puts e.name + ": " + e.message + " at line " + e.line + ", col " + e.column
	else if e.name is 'SyntaxError'
		sys.puts e.name + ": " + e.message + " at line " + e.line + ", col " + e.column
	else
		throw e
	process.exit 1
	
process.exit 0