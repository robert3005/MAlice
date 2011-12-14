fs = require 'fs'
sys = require 'sys'
util = require 'util'

parser = require '../parser.js'

directories = ['milestone3/', 'semanticTests/']

prepareTestSuite = (directory) ->
	testFilesSpec = (fs.readFileSync directory + 'semantic.spec', 'utf-8').split "\n"

	testSpec = testFilesSpec.map (fileName, index, allFiles) -> tokens = fileName.split ':'

directories.forEach ( dir, index, allDirs ) ->
	describe 'Testing semantics from directory: ' + dir , ->
		(prepareTestSuite dir).forEach (spec, index, allFiles) ->
			shouldFail = if spec[1] then 'not ' else ''
			it 'should ' + shouldFail + 'fail on ' + spec[0], ->
				file = dir + spec[0]
				data = fs.readFileSync file, 'utf-8'
				data = data.replace /[ \t\r]{2,}/g, ' '
				expectation = expect( () -> parser.parse data )
				expectation = if spec[1] then expectation.not else expectation
				expectation.toThrowName 'SemanticError'