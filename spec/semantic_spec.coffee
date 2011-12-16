fs = require 'fs'
semanticChecker = require '../semanticAnalyser.coffee'

parser = require '../parser.js'

directories = ['milestone3/', 'semanticTests/']

prepareTestSuite = (directory) ->
	testFilesSpec = (fs.readFileSync directory + 'semantic.spec', 'utf-8').split "\n"

	testSpec = testFilesSpec.map (fileName, index, allFiles) ->
							tokens = fileName.split ':'
							tokens[1] = if tokens[1] is 'pass' then yes else no
							return tokens

directories.forEach ( dir, index, allDirs ) ->
	describe 'Testing semantics from directory ' + dir , ->
		semantics = new semanticChecker
		afterEach ->
			semantics = new semanticChecker

		(prepareTestSuite dir).forEach (spec, index, allFiles) ->
			shouldFail = if spec[1] then 'not ' else ''
			it 'should ' + shouldFail + 'fail on ' + spec[0], ->
				file = dir + spec[0]
				data = fs.readFileSync file, 'utf-8'
				data = data.replace /[ \t\r]{2,}/g, ' '
				expectation = expect () -> semantics.analyse parser.parse data
				expectation = if spec[1] then expectation.not else expectation
				expectation.toThrowName 'SemanticError'