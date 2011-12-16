execScriptPath = process.argv[1]
pathTokens = execScriptPath.split '/'
pathTokens.pop()
qualifiedPath = pathTokens.join '/'

semanticChecker = require qualifiedPath + '/semanticAnalyser.coffee'
semantics = new semanticChecker
fs = require 'fs'
ffi = require 'node-ffi'
parser = require qualifiedPath + '/parser.js'

# node-ffi struct representing parse tree on C++ side
Node = ffi.Struct [
	["int32", "id"],
	["string", "type"],
	["string", "value"],
	["pointer", "position"],
	["int32", "numberOfChildren"],
	["pointer", "children"]
]

###
codeGen = new ffi.Library "./libstruct", {
	"print_struct": [ "void", [ "pointer", "int32" ] ],
	"compile": [ "void", [ "pointer", "string" ] ] 
}
###


###
Function for creating C++ struct from JavaScript parse tree.
Output is compliant with node_struct defined in structSize.h
###
parseTreeToC = (parseTree) ->
	# Object that all pointers are bound to
	# prevents C++ pointers from being garbage collected (especially null pointers)
	syntaxTree = {}

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

sourceFile = process.argv[2]

saveFileName = (source) ->
	pathTokens = sourceFile.split '/'
	fileNameTokens = pathTokens.pop().split '.'
	pathTokens.push fileNameTokens[0]
	(pathTokens.join '/') + '.ll'

# First we check whether source file to be compiled exists.
# Then we parse it, check the semantic correctness and pass 
# it to code generation to produce LLVM assembly
fs.stat sourceFile, (err, stats) ->
	
	if err
		console.error 'File: ' + sourceFile + ' doesn\'t exist'
		process.exit 1
	else
		fs.readFile sourceFile, 'utf-8', (err, source) ->
			
			if err
				console.error 'Error opening file ' + sourceFile
				process.exit 1
			
			source = source.replace /[ \t\r]{2,}/g, ' '
			try
				parseTree = parser.parse source
				semantics.analyse parseTree
				console.log parseTree
				#codeGen.compile (parseTreeToC parseTree).ref(), saveFileName arguments[0]
				process.exit 0
			catch e
				if e.name is 'SyntaxError' or e.name is 'SemanticError'
					console.error e.message + " at line: " + e.line + ", column: " + e.column + "."
				else
					console.error "Unexpected exception occured. Stack trace dumped."
					throw e
				process.exit 1