require.paths.push '/usr/lib/node_modules'
sys = require 'sys'
fs = require 'fs'
util = require 'util' # only for development and debugging\
ffi = require 'node-ffi'

syntaxTree = {}

node = ffi.Struct [
	["int32", "id"],
	["string", "type"],
	["string", "value"],
	["int32", "numberOfChildren"],
	["pointer", "children"]
]

leaf = new node
leaf.id = 1
leaf.type = "found"
leaf.value =  "0"
leaf.numberOfChildren = 0
leaf.children = new ffi.Pointer 0
leaf.children.attach syntaxTree

leaf2 = new node
leaf2.id = 2
leaf2.type = "said"
leaf2.value =  "let it be"
leaf2.numberOfChildren = 0
leaf2.children = new ffi.Pointer 0
leaf2.children.attach syntaxTree


children = new ffi.Pointer ( node.__structInfo__.size * 2 )
children.putPointer leaf, true
children.putPointer leaf2, true
children = children.seek -16
children.attach syntaxTree

root = new node
root.id = 0
root.type = "NODE_IF"
root.value = "IF"
root.numberOfChildren = 2
root.children = children
root.children.attach syntaxTree


ffiStructTest = new ffi.Library "./libstruct", {
	"print_struct": [ "void", [ "pointer", "int32" ] ]
}

ffiStructTest.print_struct root.ref(), 0