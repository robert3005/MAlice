require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

testFile = 'milestone2/ex1'

parser = PEG.buildParser fs.readFileSync 'parser_config_peg.js', 'utf-8'

jasmine.Matchers.prototype.toThrow = `function(expected) {
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

#sys.puts parser.toSource()


describe 'parser test', ->

  it 'should not fail on test 1', ->
  	fs.readFile testFile + '.alice', 'utf-8', (err, data) ->
	  	sys.puts data
  		expect(parser.parse data).not.toThrow 'SyntaxError'