Param(
    [String]$qmodname="Technicolour",
    [Parameter(Mandatory=$false)]
    [Switch]$clean
)

if ($qmodName -eq "")
{
    echo "Give a proper qmod name and try again"
    exit
}

qpm-rust qmod build --exclude_libs "libquestui.so" "libcustom-types.so" "libcodegen.so"

$mod = "./mod.json"
$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$filelist = @($mod)

$cover = "./" + $modJson.coverImage
if ((-not ($cover -eq "./")) -and (Test-Path $cover))
{
    $filelist += ,$cover
}

foreach ($modFile in $modJson.modFiles)
{
        $path = "./build/" + $modFile
    if (-not (Test-Path $path))
    {
        $path = "./extern/libs/" + $modFile
    }
    $filelist += $path
}

foreach ($lib in $modJson.libraryFiles)
{
    $path = "./extern/libs/" + $lib
    if (-not (Test-Path $path))
    {
        $path = "./build/" + $lib
    }
    $filelist += $path
}


$zip = $qmodName + ".zip"
$qmod = $qmodName + ".qmod"

if ((-not ($clean.IsPresent)) -and (Test-Path $qmod))
{
    echo "Making Clean Qmod"
    Move-Item $qmod $zip -Force
}

Compress-Archive -Path $filelist -DestinationPath $zip -Update
Move-Item $zip $qmod -Force
Remove-Item $mod
Write-Output "QMOD saved to $qmod"