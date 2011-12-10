require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

directories = ['milestone2/', 'milestone3/']
extension = '.alice'

parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'


prepareTestSuite = (directory) ->
	testFilesSpec = fs.readFileSync directory + 'parserTests.spec', 'utf-8'

	testFiles = fs.readdirSync(directory);

	testSpec = testFiles.filter((file) ->
				file.substr(-6) is extension
			).sort((a, b) ->
				fileNumberA = parseInt ((a.split '.alice')[0].split 'ex')[1]
				fileNumberB = parseInt ((b.split '.alice')[0].split 'ex')[1]
				# Have fun reading it, I know everyone will enjoy it. Looks like Haskell, doesn't it?
				result = if isNaN fileNumberA then 1 else if isNaN fileNumberB then -1 else if fileNumberA < fileNumberB then -1 else if fileNumberA == fileNumberB then 0 else 1
			).map((fileName, index, allFiles) ->
				regex = new RegExp fileName + ':pass', 'ig'
				pass = testFilesSpec.match(regex)?
				[fileName, pass]
			)

###
beforeEach () -> 
	this.addMatchers
		toThrowName: (expected) ->
			result = false
			exception
			throw new Error('Actual is not a function') if typeof this.actual isnt 'function'
			try
				this.actual()
			catch e
				exception = e

			result = (expected is jasmine.undefined or this.env.equals_(exception.message or exception, expected.message or expected) or this.env.equals_(exception.name, expected)) if exception

			shouldNot = if this.isNot then "not " else ""

			this.message = () ->
				if exception and (expected is jasmine.undefined or not this.env.equals_(exception.message or exception, expected.message or expected))
					return ["Expected function " + shouldNot + "to throw"
					 (if expected then expected.name or expected.message or expected else " an exception")
					 ", but it threw"
					 (exception.name or exception.message or exception)].join(' ')
				else
					return "Expected function to throw an exception."

			return result
###

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