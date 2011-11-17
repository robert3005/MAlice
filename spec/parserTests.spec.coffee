require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

directory = 'milestone2/'
extension = '.alice'

parser = PEG.buildParser fs.readFileSync 'parser_config_peg.js', 'utf-8'

testFilesSpec = fs.readFileSync directory + 'parserTests.spec', 'utf-8'

testFiles = fs.readdirSync(directory);

testSpec = testFiles.filter((file) ->
			file.substr(-6) is extension
		).sort((a, b) ->
			fileNumberA = parseInt ((a.split '.alice')[0].split 'ex')[1]
			fileNumberB = parseInt ((b.split '.alice')[0].split 'ex')[1]
			# Have fun reading it, I know everyone will enjoy it. Looks like Haskell doesn't it?
			result = if isNaN fileNumberA then 1 else if isNaN fileNumberB then -1 else if fileNumberA < fileNumberB then -1 else if fileNumberA == fileNumberB then 0 else 1
		).map((fileName, index, allFiles) ->
			regex = new RegExp fileName + ':pass', 'ig'
			pass = testFilesSpec.match(regex)?
			[fileName, pass]
		)

#sys.puts testSpec

beforeEach () -> 
	this.addMatchers
		toThrowName: `function(expected) {
			var result = false;
			var exception;
			if (typeof this.actual != 'function') {
			throw new Error('Actual is not a function');
			}
			try {
			this.actual();
			} catch (e) {
			exception = e;
			}
			if (exception) {
			  result = (expected === jasmine.undefined || this.env.equals_(exception.message || exception, expected.message || expected) || this.env.equals_(exception.name, expected));
			}

			var not = this.isNot ? "not " : "";

			this.message = function() {
			if (exception && (expected === jasmine.undefined || !this.env.equals_(exception.message || exception, expected.message || expected))) {
			  return ["Expected function " + not + "to throw", expected ? expected.name || expected.message || expected : " an exception", ", but it threw", exception.name || exception.message || exception].join(' ');
			} else {
			  return "Expected function to throw an exception.";
			}
			};

			return result;
			}`

describe 'parser test', ->

	testSpec.forEach (spec, index, allFiles) ->
		shouldFail = if spec[1] then 'not ' else ''
		it 'should ' + shouldFail + 'fail on ' + spec[0], ->
			file = directory + spec[0]
			data = fs.readFileSync file, 'utf-8'
			data = data.replace /[ \t\r]{2,}/g, ' '
			expectation = expect( () -> parser.parse data )
			expectation = if spec[1] then expectation.not else expectation
			expectation.toThrowName 'SyntaxError'