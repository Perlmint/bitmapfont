{
  'conditions': [
    ['OS=="win"', {
      'variables': {
        'MAGICK_ROOT%': '<!(python get_regvalue.py)',
        # download the dll binary and check off for libraries and includes
        'OSX_VER%': "0",
      }
    }],
    ['OS=="mac"', {
      'variables': {
        # matches 10.9.X , 10.10 and outputs 10.9 , 10.10, 10.11
        'OSX_VER%': "<!(sw_vers | grep 'ProductVersion:' | grep -o '10.[0-9]*')",
      }
    }, {
      'variables': {
        'OSX_VER%': "0",
      }
    }]
  ],
  "targets": [
    {
      "target_name": "bitmapfont",
      "sources": [
        "src/addon.cc",
        "src/bitmapfont.cc",
        "src/canvas.cc",
        "include/bitmapfont.hpp",
        "include/canvas.hpp",
        "include/node_helper.h"
      ],
      'defines': [
        'NODE'
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "include"
      ],
      "conditions": [
        ['OS=="win"', {
          "libraries": [
            '-l<(MAGICK_ROOT)/lib/CORE_RL_MagickCore_.lib',
            '-l<(MAGICK_ROOT)/lib/CORE_RL_MagickWand_.lib',
          ],
          'include_dirs': [
            '<(MAGICK_ROOT)/include',
          ]
        }],
        ['OS=="win" and target_arch!="x64"', {
          'defines': [
            '_SSIZE_T_',
          ]
        }],
        ['OSX_VER == "10.9" or OSX_VER == "10.10" or OSX_VER == "10.11" or OSX_VER == "10.12"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '<!@(Magick++-config --cflags)'
            ],
            'OTHER_CPLUSPLUSFLAGS' : [
              '<!@(Magick++-config --cflags)',
              '-std=c++11',
              '-stdlib=libc++',
            ],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7', # -mmacosx-version-min=10.7
          },
          "libraries": [
             '<!@(Magick++-config --ldflags --libs)',
          ],
          'cflags': [
            '<!@(Magick++-config --cflags --cppflags)'
          ],
        }],
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '<!@(Magick++-config --cflags)'
            ]
          },
          "libraries": [
             '<!@(Magick++-config --ldflags --libs)',
          ],
          'cflags': [
            '<!@(Magick++-config --cflags --cppflags)'
          ],
        }],
        ['OS=="linux" or OS=="solaris" or OS=="freebsd"', { # not windows not mac
          "libraries": [
            '<!@(Magick++-config --ldflags --libs)',
          ],
          'cflags': [
            '<!@(Magick++-config --cflags --cppflags)'
          ],
        }]
      ]
    }]
  }