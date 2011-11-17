require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

testFile = '../milestone2/ex1'

fs.readFile '../parser_config_peg.js', 'utf-8', (err, data) ->
  parser = PEG.buildParser data


describe 'parser test', () ->
  it 'should not fail on test 1', () ->
    fs.readFile testFile + '.alice', 'utf-8', (err, data) ->
      expect(parser.parse data).not.toThrow 'SyntaxError'