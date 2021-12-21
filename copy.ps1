& $PSScriptRoot/build.ps1
if ($?) {
    adb push build/libtechnicolour.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libtechnicolor.so
    if ($?) {
        & $PSScriptRoot/restart-game.ps1
        if ($args[0] -eq "--log") {
            & $PSScriptRoot/start-logging.ps1
        }
    }
}
