parser = require './parser.js'
semantics = require './semanticAnalyser.coffee'
sys = require 'sys'
fs = require 'fs'

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /([ ]{2,}|[\t\r]+)/g, ' '

try
	parseTree = parser.parse source
	console.log parseTree
	labelTree = semantics.analyse parseTree
catch e
	#if e instanceof parser.SyntaxError
		#We've encountered Syntax error
	#if e instanceof semantics.SemanticError
		#We've encountered semantic error
	throw e

console.log parseTree[1]

sys.puts semantics.changeToString parseTree[1], 0


#console.log labelTree