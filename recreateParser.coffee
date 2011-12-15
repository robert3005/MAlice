require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'
parserFileName = 'parser.js'

parserModule = "module.exports=" + parser.toSource()

fs.realpath parserFileName, (err, resolvedPath) ->
	if err
		fs.writeFile parserFileName, parser.toSource(), 'utf8', (err) ->
			throw err if err 
			console.log('New parser written')
	else
		fs.unlink parserFileName, (err) ->
				throw err if err
				console.log('Old parser removed')
				fs.writeFile parserFileName, parserModule, 'utf8', (err) ->
					throw err if err
					console.log('New parser written')