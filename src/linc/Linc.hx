package linc;

import haxe.io.Path;
import haxe.macro.Expr;
import haxe.macro.Context;

using haxe.macro.PositionTools;

class Linc
{
    /**
     * Adds a private internal inline static variable called __touch,
     * which sets the value to the current time so that builds are always
     * updated by the code, and native changes are dragged in automatically (except for header only changes)
     */
    macro public static function touch() : Array<Field>
    {
        final fields = Context.getBuildFields();

        fields.push({
            name   : '__touch',
            pos    : Context.currentPos(),
            doc    : null,
            meta   : [],
            access : [APrivate, AStatic, AInline],
            kind   : FVar(macro : String, macro $v{ Std.string(Date.now().getTime()) }),
        });

        return fields;
    }

    /**
     * Adds a @:buildXml meta node with a linc <set> and an <import> tag.
     * The set is named LINC_${_lib}_PATH, and points to the root folder of the library.
     * That path is calculated from the calling file using the optional _relative_root, default ../
     * This means that somelib/ is the root.
     * somelib/somelib/Somelib.hx is the calling file.
     * LINC_SOMELIB_PATH is set to somelib/
     * ${LINC_SOMELIB_PATH}/linc/linc_${_lib}.xml is added directly.
     */
    macro public static function xml(_lib : String) : Array<Field>
    {
        final pos     = Context.currentPos();
        final posInfo = pos.getInfos();
        final clazz   = Context.getLocalClass();

        var sourcePath = Path.directory(posInfo.file);
        if (!Path.isAbsolute(sourcePath))
        {
            sourcePath = Path.join([ Sys.getCwd(), sourcePath ]);
        }

        sourcePath = Path.normalize(sourcePath);

        final xmlPath = Path.normalize(Path.join([ sourcePath, '${_lib}.xml' ]));
        final include = '<include name="$xmlPath" />';

        clazz.get().meta.add(":buildXml", [{ expr : EConst(CString(include)), pos : pos }], pos );
        
        return Context.getBuildFields();

    }
}