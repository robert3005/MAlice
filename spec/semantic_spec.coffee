fs = require 'fs'
sys = require 'sys'
util = require 'util'

directory = 'semanticTests/'

console.log process.cwd()
console.log __dirname, __filename

parser = require '../parser.js'

testFilesSpec = (fs.readFileSync directory + '.spec', 'utf-8').split "\n"

testSpec = testFilesSpec.map (fileName, index, allFiles) -> tokens = fileName.split ':'

describe 'Testing semantics from directory: ' + directory , ->
	testSpec.forEach (spec, index, allFiles) ->
		shouldFail = if spec[1] then 'not ' else ''
		it 'should ' + shouldFail + 'fail on ' + spec[0], ->
			file = directory + spec[0]
			data = fs.readFileSync file, 'utf-8'
			data = data.replace /[ \t\r]{2,}/g, ' '
			expectation = expect( () -> parser.parse data )
			expectation = if spec[1] then expectation.not else expectation
			expectation.toThrowName 'SemanticError'