#!/usr/bin/env bash

# Interdit l'utilisation de variables non initialisées et les erreurs
set -o nounset
set -o errexit
set -o pipefail

gcc -Wall -Wextra -Werror \
	$(pkg-config --cflags gtk+-3.0) \
	main.c \
	$(pkg-config --libs gtk+-3.0) \
	-o gtk-echec

./gtk-echec
