require.paths.push '/usr/lib/node_modules'
PEG = require 'pegjs'
fs = require 'fs'
sys = require 'sys'

parser = PEG.buildParser fs.readFileSync 'pegParserConfig.js', 'utf-8'
parserFileName = 'parser.js'
oldParserFileName = 'parser.old.js'
		
fs.realpath parserFileName, (err, resolvedPath) ->
	if err
		fs.writeFile parserFileName, parser.toSource(), 'utf8', (err) ->
			throw err if err 
			console.log('New parser written')
	else
		fs.realpath oldParserFileName, (err, resolvedOldPath) ->
			if not err
				fs.unlink oldParserFileName, (err) ->
					throw err if err
					console.log('Old parser removed')
			fs.rename parserFileName, oldParserFileName
			fs.writeFile parserFileName, parser.toSource(), 'utf8', (err) ->
				throw err if err
				console.log('New parser written')