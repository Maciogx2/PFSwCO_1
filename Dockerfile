#Maciej Rutkowski
from ubuntu as compile
ADD server.c /server.c
RUN apt-get update
RUN apt-get install -y musl-tools
RUN musl-gcc -Os --static -s -fno-stack-protector -ffunction-sections -fdata-sections -Wl,--gc-sections server.c -o server
RUN strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-ta server
 
FROM scratch
COPY --from=compile /server /server
EXPOSE 3000
CMD ["/server"]