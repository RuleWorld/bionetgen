# -*- mode: python -*-
a = Analysis(['sbmlTranslator.py'],
             hiddenimports=['_libsbml',],
             hookspath=None,
             runtime_hooks=None,
              excludes=['PyQt4', 'PyQt4.QtCore', 'PyQt4.QtGui','matplotlib','IPython','PIL'])

a.datas += [('config\\namingConventions.json','config\\namingConventions.json','DATA'),
('config\\reactionDefinitions.json','config\\reactionDefinitions.json','DATA')]
pyz = PYZ(a.pure)


exe = EXE(pyz,
          a.scripts + [('O','','OPTION')],
          a.binaries,
          Tree('.\\reactionDefinitions', prefix = 'reactionDefinitions'),
          [('stats4.npy', 'stats4.npy', 'DATA')],
          a.zipfiles,
          a.datas,
          name='sbmlTranslator.exe',
          debug=False,
          strip=None,
          upx=True,
          console=True )
