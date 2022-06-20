package vcpkg;

import sys.FileSystem;
import haxe.io.Path;
import haxe.macro.Expr;
import haxe.macro.Context;
import haxe.macro.Compiler;

using Lambda;
using haxe.macro.PositionTools;

class Vcpkg
{
    public static macro function install(_install : String)
    {
#if !display
        final isDebug    = Context.defined('debug');
        final vcpkgDir   = Path.join([ Sys.getCwd(), _install ]);
        final includeDir = Path.join([ vcpkgDir, 'include' ]);
        final libsDir    = if (isDebug) Path.join([ vcpkgDir, 'debug', 'lib' ]) else Path.join([ vcpkgDir, 'lib' ]);
        final binDir     = if (isDebug) Path.join([ vcpkgDir, 'debug', 'bin' ]) else Path.join([ vcpkgDir, 'bin' ]);
        final libs       =
            FileSystem
                .readDirectory(libsDir)
                .map(rel -> Path.join([ libsDir, rel ]))
                .filter(abs -> !FileSystem.isDirectory(abs))

                .map(abs -> '<lib name="${ abs }"/>')
                .join('');

        final buildXml   = '<files id="haxe"><compilerflag value="-I${ includeDir }"/></files><files id="vcpkg"><compilerflag value="-I${ includeDir }"/></files><target id="haxe"><files id="vcpkg"/>${libs}</target>';
        final timeNow    = Std.string(Date.now().getTime());
        final forceClass = macro class VcpkgInstall {
            public static inline var __touch = $v{ timeNow };
        };

        forceClass.meta = [
            { name : ':buildXml', params : [ macro $v{ buildXml } ], pos : Context.currentPos() },
            { name : ':keep', pos : Context.currentPos() }
        ];

        Context.defineType(forceClass);
        Compiler.include('', false, null, [ 'Vcpkg' ]);

        Context.onAfterGenerate(() -> {
            for (file in FileSystem.readDirectory(binDir))
            {
                sys.io.File.copy(Path.join([ binDir, file ]), Path.join([ Compiler.getOutput(), file ]));
            }
        });
#end
        return macro null;
    }
}