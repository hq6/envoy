FROM jaegertracing/all-in-one@sha256:f8aefa8134154cd52c8b2c27031c8353e5dd6f56a077d66250cb6d06d4d236f7
HEALTHCHECK \
    --interval=1s \
    --timeout=1s \
    --start-period=1s \
    --retries=60 \
    CMD wget -q --header='Content-Type:application/json' -O - http://localhost:14269/health | grep "Server available"
