parser = require './parser.js'
semantics = require './semanticAnalyser.coffee'
sys = require 'sys'
fs = require 'fs'

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '

try
	parseTree = parser.parse source
	labelTree = semantics.analyse parseTree
catch e
	throw e
	process.exit 1

sys.puts semantics.buildtree parseTree

process.exit 0