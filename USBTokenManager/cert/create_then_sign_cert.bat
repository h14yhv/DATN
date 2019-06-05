REM PLEASE UPDATE THE FOLLOWING VARIABLES FOR YOUR NEEDS.
SET HOSTNAME=gateway.vna.svpgate.com
SET HOSTNAME_FILE=svpgate.com

SET COUNTRY=VN
SET STATE=Hanoi
SET CITY=Hanoi
SET ORGANIZATION=VNA
SET ORGANIZATION_UNIT=VNA
SET EMAIL=webmaster@%HOSTNAME%
SET CA=VNA_SVP.pem
SET CA_KEY=VNA_SVP-key.pem
SET SERIAL=3

(
echo [req]
echo default_bits = 2048
echo prompt = no
echo default_md = sha256
echo req_extensions = v3_req
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
echo:
echo [alt_names]
echo DNS.1 = *.%HOSTNAME%
echo DNS.2 = %HOSTNAME%
)>%HOSTNAME_FILE%.cnf

openssl req -new -sha256 -nodes -out %HOSTNAME_FILE%.csr -newkey rsa:2048 -keyout %HOSTNAME_FILE%.key -config %HOSTNAME_FILE%.cnf
openssl x509 -req -in %HOSTNAME_FILE%.csr -CA %CA% -CAkey %CA_KEY% -CAcreateserial -out %HOSTNAME_FILE%.crt -days 3650 -extfile %HOSTNAME_FILE%.cnf -extensions v3_req
del /f %HOSTNAME_FILE%.csr
del /f %HOSTNAME_FILE%.cnf