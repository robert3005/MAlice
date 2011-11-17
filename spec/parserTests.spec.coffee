require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

testFile = '../milestone2/ex1'

fs.readFile '../parser_config_peg.js', 'utf-8', (err, data) ->
  parser = PEG.buildParser data
  sys.puts parser.toSource()


describe 'parser test', ->

  it 'should not fail on test 1', ->
    fs.readFile testFile + '.alice', 'utf-8', (err, data) ->
	    expect(parser.parse data).not.toThrow 'SyntaxError'

describe 'Math:', ->
  describe 'fib()', ->
    it 'should calculate the numbers correctly up to fib(16)', ->
      fib = [0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987]
      expect(Math.fib(i)).toEqual fib[i] for i in [0..16]