fs = require 'fs'
sys = require 'sys'

describe 'parser test', ->

  it 'should not fail on test 1', ->
    fs.readFile testFile + '.alice', 'utf-8', (err, data) ->
    	throw err if err
    	sys.puts data
	    expect(parser.parse data).not.toThrow 'SyntaxError'