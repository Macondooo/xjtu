"""
Your awesome Distance Vector router for CS 168

Based on skeleton code by:
  MurphyMc, zhangwen0411, lab352
"""

import sim.api as api
from cs168.dv import RoutePacket, \
    Table, TableEntry, \
    DVRouterBase, Ports, \
    FOREVER, INFINITY


class DVRouter(DVRouterBase):

    # A route should time out after this interval
    ROUTE_TTL = 15

    # Dead entries should time out after this interval
    GARBAGE_TTL = 10

    # -----------------------------------------------
    # At most one of these should ever be on at once
    SPLIT_HORIZON = False
    POISON_REVERSE = True
    # -----------------------------------------------

    # Determines if you send poison for expired routes
    POISON_EXPIRED = True

    # Determines if you send updates when a link comes up
    SEND_ON_LINK_UP = False

    # Determines if you send poison when a link goes down
    POISON_ON_LINK_DOWN = False

    def __init__(self):
        """
        Called when the instance is initialized.
        DO NOT remove any existing code from this method.
        However, feel free to add to it for memory purposes in the final stage!
        """
        assert not (self.SPLIT_HORIZON and self.POISON_REVERSE), \
            "Split horizon and poison reverse can't both be on"

        self.start_timer()  # Starts signaling the timer at correct rate.

        # Contains all current ports and their latencies.
        # See the write-up for documentation.
        self.ports = Ports()

        # This is the table that contains all current routes
        self.table = Table()
        self.table.owner = self

        self.history = {}  # port->dst->tuple
        self.copy = {}

        # add myself

    def add_static_route(self, host, port):
        """
        Adds a static route to this router's table.

        Called automatically by the framework whenever a host is connected
        to this router.

        :param host: the host.
        :param port: the port that the host is attached to.
        :returns: nothing.
        """
        # `port` should have been added to `peer_tables` by `handle_link_up`
        # when the link came up.s
        assert port in self.ports.get_all_ports(
        ), "Link should be up, but is not."

        # TODO: fill this in!
        self.table[host] = TableEntry(
            dst=host, port=port, latency=self.ports.get_latency(port), expire_time=FOREVER)

        self.send_routes(force=False)

    def handle_data_packet(self, packet, in_port):
        """
        Called when a data packet arrives at this router.

        You may want to forward the packet, drop the packet, etc. here.

        :param packet: the packet that arrived.
        :param in_port: the port from which the packet arrived.
        :return: nothing.
        """
        # TODO: fill this in!
        if packet.dst in self.table and self.table[packet.dst][2] < INFINITY:
            self.send(packet=packet, port=self.table[packet.dst][1])

    def send_routes(self, force=False, single_port=None):
        """
        Send route advertisements for all routes in the table.

        :param force: if True, advertises ALL routes in the table;
                      otherwise, advertises only those routes that have
                      changed since the last advertisement.
               single_port: if not None, sends updates only to that port; to
                            be used in conjunction with handle_link_up.
        :return: nothing.
        """
        # TODO: fill this in!
        self.copy = {}
        for port in self.ports.get_all_ports():
            self.copy[port] = {}
        # print(copy.keys())

        if force:
            if self.SPLIT_HORIZON:
                for entry in self.table.values():
                    for port in self.ports.get_all_ports():
                        p = RoutePacket(destination=entry[0], latency=entry[2])
                        if port != entry[1]:
                            self.send(packet=p, port=port, flood=False)
                            self.renew_history(
                                packet=p, flood=False, port=port)
            elif self.POISON_REVERSE:
                for entry in self.table.values():
                    p1 = RoutePacket(
                        destination=entry[0], latency=INFINITY)
                    if entry[1] in self.ports.get_all_ports():
                        self.send(packet=p1, port=entry[1], flood=False)
                        self.renew_history(
                            packet=p1, flood=False, port=entry[1])

                    p2 = RoutePacket(
                        destination=entry[0], latency=entry[2])
                    for port in self.ports.get_all_ports():
                        if port != entry[1]:
                            self.send(packet=p2, port=port, flood=False)
                            self.renew_history(
                                packet=p2, flood=False, port=port)
            else:
                if single_port == None:
                    for entry in self.table.values():
                        for port in self.ports.get_all_ports():
                            p = RoutePacket(
                                destination=entry[0], latency=entry[2])
                            self.send(packet=p, port=port, flood=False)
                            self.renew_history(
                                packet=p, port=port, flood=False)
                else:  # handle link up
                    for entry in self.table.values():
                        p = RoutePacket(destination=entry[0], latency=entry[2])
                        self.send(packet=p, port=single_port, flood=False)
                        self.renew_history(
                            packet=p, flood=False, port=single_port)

        else:
            if self.SPLIT_HORIZON:
                for entry in self.table.values():
                    for port in self.ports.get_all_ports():
                        if entry[1] != port and (entry[0] not in self.history[port] or entry[2] != self.history[port][entry[0]]):
                            p = RoutePacket(
                                destination=entry[0], latency=entry[2])
                            self.send(p, port=port, flood=False)
                            self.renew_history(
                                packet=p, flood=False, port=port)

            elif self.POISON_REVERSE:
                for entry in self.table.values():
                    for port in self.ports.get_all_ports():
                        if entry[1] != port and (entry[0] not in self.history[port] or entry[2] != self.history[port][entry[0]]):
                            p1 = RoutePacket(
                                destination=entry[0], latency=entry[2])
                            self.send(p1, port=port, flood=False)
                            self.renew_history(
                                packet=p1, flood=False, port=port)
                        elif entry[1] == port and (entry[0] not in self.history[port] or INFINITY != self.history[port][entry[0]]):
                            p2 = RoutePacket(
                                destination=entry[0], latency=INFINITY)
                            self.send(p2, port=port, flood=False)
                            self.renew_history(
                                packet=p2, flood=False, port=port)
            else:
                for entry in self.table.values():
                    for port in self.ports.get_all_ports():
                        if entry[0] not in self.history[port] or entry[2] != self.history[port][entry[0]]:
                            p = RoutePacket(
                                destination=entry[0], latency=entry[2])
                            self.send(p, port=port, flood=False)
                            self.renew_history(
                                packet=p, flood=False, port=port)

        for key in self.copy:
            if self.copy[key]:
                self.history[key] = self.copy[key]

    def expire_routes(self):
        """
        Clears out expired routes from table.
        accordingly.
        """
        # TODO: fill this in!
        for key in list(self.table.keys()):
            entry = self.table[key]
            if entry.has_expired:
                if self.POISON_EXPIRED:
                    self.table[key] = TableEntry(
                        dst=entry[0], port=entry[1], latency=INFINITY, expire_time=api.current_time()+self.ROUTE_TTL)
                else:
                    self.s_log(
                        "the route to %s on port %s has been clear out", entry[0], entry[1])
                    del self.table[key]

    def handle_route_advertisement(self, route_dst, route_latency, port):
        """
        Called when the router receives a route advertisement from a neighbor.

        :param route_dst: the destination of the advertised route.
        :param route_latency: latency from the neighbor to the destination.
        :param port: the port that the advertisement arrived on.
        :return: nothing.
        """
        # TODO: fill this in!
        f1 = False
        f2 = False
        f3 = False
        # not in
        # check if expired?
        if route_latency >= INFINITY:
            if route_dst in self.table and port == self.table[route_dst][1]:
                if self.table[route_dst][2] < INFINITY:
                    self.table[route_dst] = TableEntry(
                        dst=route_dst, port=port, latency=INFINITY, expire_time=api.current_time()+self.ROUTE_TTL)
                    self.send_routes(force=False)

        else:
            f1 = route_dst not in self.table
            if not f1:
                f2 = self.table[route_dst][2] > route_latency + \
                    self.ports.get_latency(port)
                f3 = self.table[route_dst][1] == port
            if f1 or f2 or f3:
                self.table[route_dst] = TableEntry(
                    dst=route_dst, port=port, latency=route_latency+self.ports.get_latency(port), expire_time=api.current_time()+self.ROUTE_TTL)
                self.send_routes(force=False)

    def handle_link_up(self, port, latency):
        """
        Called by the framework when a link attached to this router goes up.

        :param port: the port that the link is attached to.
        :param latency: the link latency.
        :returns: nothing.
        """
        # print(self.history.keys())
        self.ports.add_port(port, latency)
        self.history[port] = {}
        # print('add ', port)
        # print(self.history.keys())

        # TODO: fill in the rest!
        if self.SEND_ON_LINK_UP:
            self.send_routes(force=True, single_port=port)

    def handle_link_down(self, port):
        """
        Called by the framework when a link attached to this router does down.

        :param port: the port number used by the link.
        :returns: nothing.
        """

        self.ports.remove_port(port)
        del self.history[port]

        # TODO: fill this in!

        if self.POISON_ON_LINK_DOWN:
            for key in list(self.table.keys()):
                if self.table[key][1] == port:
                    entry = self.table[key]
                    self.table[key] = TableEntry(
                        entry[0], entry[1], INFINITY, entry[3])
            self.send_routes(force=False)
        else:
            for key in list(self.table.keys()):
                if self.table[key][1] == port:
                    del self.table[key]

    # Feel free to add any helper methods!
    def renew_history(self, packet, flood=True, port=None):
        # if all = TRUE , renew all ports except port
        # else only renew port
        if flood:
            if port == None:  # send to all ports
                for key in self.copy.keys():
                    self.copy[key][packet.destination] = packet.latency
            else:
                for key in self.copy.keys():  # send to all ports except port
                    if key != port:
                        self.copy[key][packet.destination] = packet.latency
        else:  # only send to port
            self.copy[port][packet.destination] = packet.latency
