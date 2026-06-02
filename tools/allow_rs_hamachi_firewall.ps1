$ErrorActionPreference = "Stop"

$ports = @(53621, 54631, 55661, 56621)

foreach ($port in $ports) {
    $name = "RS Emu Hamachi TCP $port"

    if (-not (Get-NetFirewallRule -DisplayName $name -ErrorAction SilentlyContinue)) {
        New-NetFirewallRule `
            -DisplayName $name `
            -Direction Inbound `
            -Action Allow `
            -Protocol TCP `
            -LocalPort $port `
            -RemoteAddress 25.0.0.0/8 `
            -Profile Any | Out-Null
    }
}

Set-NetConnectionProfile -InterfaceAlias "Hamachi" -NetworkCategory Private

"OK $(Get-Date -Format s)" | Set-Content -Path (Join-Path $PSScriptRoot "allow_rs_hamachi_firewall.log")
