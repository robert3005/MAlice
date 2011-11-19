parser = require './parser.js'
semantics = require './semanticAnalyzer.coffee'
sys = require 'sys'
fs = require 'fs'

arguments = process.argv.splice 2

source = fs.readFileSync arguments[0], 'utf-8'
source = source.replace /[ \t\r]{2,}/g, ' '

parseTree = parser.parse source

console.log parseTree