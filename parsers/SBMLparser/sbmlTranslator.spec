# -*- mode: python -*-
a = Analysis(['sbmlTranslator.py'],
             pathex=['/home/proto/workspace/bionetgen/parsers/SBMLparser'],
             hiddenimports=['_libsbml'],
             hookspath=None,
             runtime_hooks=None,
	      excludes=['PyQt4', 'PyQt4.QtCore', 'PyQt4.QtGui'])
pyz = PYZ(a.pure)


#dict_tree = Tree('./reactionDefinitions', prefix = 'reactionDefinitions')
#dict_tree += [('stats4.npy', 'stats4.npy', 'DATA')]

exe = EXE(pyz,
          a.scripts + [('O','','OPTION')],
	  a.binaries,
	  Tree('./reactionDefinitions', prefix = 'reactionDefinitions'),
	  [('stats4.npy', 'stats4.npy', 'DATA')],
	  a.zipfiles,
          a.datas,
          name='sbmlTranslator',
          debug=False,
          strip=None,
          upx=True,
          console=True )


#coll = COLLECT(exe,
#               a.binaries,
#	       dict_tree,
#               a.zipfiles,
#               a.datas,
#               strip=None,
 #              upx=True,
 #              name='sbmlTranslator')
