import io from 'socket.io-client';

export const socket_url = `http://localhost:8080`;

const socket = io(socket_url, { reconnectionDelay: 4000 });
export default socket;
