REM PLEASE UPDATE THE FOLLOWING VARIABLES FOR YOUR NEEDS.
SET HOSTNAME=CA
SET HOSTNAME_FILE=CA
SET COUNTRY=VN
SET STATE=Hanoi
SET CITY=Hanoi
SET ORGANIZATION=VNA
SET ORGANIZATION_UNIT=SVP
SET EMAIL=webmaster@svpgate.com

(
echo [req]
echo default_bits = 2048
echo prompt = no
echo default_md = sha256
echo x509_extensions = v3_req
echo distinguished_name = dn
echo:
echo [dn]
echo C = %COUNTRY%
echo ST = %STATE%
echo L = %CITY%
echo O = %ORGANIZATION%
echo OU = %ORGANIZATION_UNIT%
echo emailAddress = %EMAIL%
echo CN = %HOSTNAME%
echo:
echo [v3_req]
echo subjectAltName = @alt_names
echo basicConstraints = CA:TRUE
echo:
echo [alt_names]
echo DNS.1 = %HOSTNAME%
REM echo DNS.2 = *.%HOSTNAME%
)>%HOSTNAME_FILE%.cnf

openssl req -new -x509 -newkey rsa:2048 -sha256 -nodes -keyout %HOSTNAME_FILE%-key.pem -days 3560 -out %HOSTNAME_FILE%.pem -config %HOSTNAME_FILE%.cnf
del /f "%HOSTNAME_FILE%.cnf"