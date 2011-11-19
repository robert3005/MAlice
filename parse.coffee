parser = require './parser.js'
semantics = require './semanticAnalyzer.coffee'
sys = require 'sys'
fs = require 'fs'

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '

try
	parseTree = parser.parse source
	labelTree = semantics.analyze parseTree
catch e
	if e instanceof parser.SyntaxError
		e
		#We've encountered Syntax error
	if e instanceof semantics.SemanticError
		e
		#We've encountered semantic error
	console.log e


console.log parseTree