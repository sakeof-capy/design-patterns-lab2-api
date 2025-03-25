# Capybara API

## Clone the Repo
Recursive clone is required:
```bash
git clone --required git@github.com:sakeof-capy/design-patterns-lab2-api.git
```
or 
```bash
git clone --required https://github.com/sakeof-capy/design-patterns-lab2-api.git
```

## API Docs
The swagger file can be found here: `./swagger-docs.yaml`.

## Build & Run
Create `.env` file and fill it with values:
```bash
cp env-template .env
```

To build and start the server, run the following command:
```bash
docker-compose up
```

## Details
The port, the server will be listening on, can be specified in .env file: 
```
SERVER_PORT=<your_port>
```
