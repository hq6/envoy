FROM node:21.2-bullseye-slim@sha256:7ceb032d9ffe90538cd140d5da9dd26ac24994f23daa00e757a4718fa377d171 as node-base


FROM node-base as node-http-auth
ARG NODE_APP=.
ARG NODE_APP_NAME=http-service/server
# Add an env to save ARG
ENV NODE_APP_PATH "/app/${NODE_APP_NAME}"
COPY "$NODE_APP" /app
# Dont use exec form to interpolate correctly
CMD node $NODE_APP_PATH
