require.paths.push '/usr/lib/node_modules'
require.paths.push '/usr/local/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'
util = require 'util'

directories = ['milestone2/', 'milestone3/', 'milestone4/']

parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'


prepareTestSuite = (directory) ->
	testFilesSpec = (fs.readFileSync directory + 'parser.spec', 'utf-8').split "\n"

	testSpec = testFilesSpec.map (fileName, index, allFiles) -> tokens = fileName.split ':'

directories.forEach ( dir, index, allDirs ) ->
	describe 'Parsing sample files from: ' + dir , ->
		(prepareTestSuite dir).forEach (spec, index, allFiles) ->
			shouldFail = if spec[1] then 'not ' else ''
			it 'should ' + shouldFail + 'fail on ' + spec[0], ->
				file = dir + spec[0]
				data = fs.readFileSync file, 'utf-8'
				data = data.replace /[ \t\r]{2,}/g, ' '
				expectation = expect( () -> parser.parse data )
				expectation = if spec[1] then expectation.not else expectation
				expectation.toThrowName 'SyntaxError'
