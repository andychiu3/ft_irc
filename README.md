<h1 align="center"> ft_IRC (Internet Relay Chat) </h1>

## how to use

Get the repo from git:
```bash
git clone <URL> ft_irc
```

Change Directory and compile:
```bash
cd ft_irc && make
```

Run the program:
```bash
./ft_irc <port> <password>
```

Connect with chosen IRC client using the specified `port` and `password`

p.s. ERC should be available with both Linux and macOS so that is the one we chose but it may have a steeper learning curve.

<br>

## Features

The server implements essential IRC commands including:

`invite` `join` `kick` `list` `mode` `names` `nick` `notice` `part` `pass` `ping` `privmsg` `quit` `topic` `user`

with all this commands we can expect:

- Multi-client connection support
  
- Channel management with operators
  
- Private messaging between users
  
- Server authentication with password protection
  
- Real-time message broadcasting

<br>

## Notes

This is a collaborative 42 project developed with [@jmaio22](https://github.com/jmaio22)

**Team contributions:**

[@jmaio22](https://github.com/jmaio22): Built the fundamental server architecture and client connection handling

me:  Implemented IRC commands, channel management and server replies

<br>

## Client compatibility:

Our primary testing client is ERC (Emacs IRC client), which has a steeper learning curve but offers robust functionality. 

The server also communicates seamlessly with LimeChat on macOS for users preferring a more user-friendly interface.

For quick testing and debugging, `netcat` or `telnet` can be used to connect directly to the IRC server.

<br>

## Technical considerations:

This project follows 42 constraints with limited function access and specific coding standards, 

so the implementation focuses on core IRC functionality rather than advanced features found in production IRC servers.

<br>

## References

- [RFC 1459 - Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)
- [RFC 2812 - Internet Relay Chat: Client Protocol](https://www.rfc-editor.org/rfc/rfc2812)
