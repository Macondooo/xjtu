import sim


def launch(switch_type=sim.config.default_switch_type, host_type=sim.config.default_host_type):
    # my topo
    switch_type.create('A')
    switch_type.create('B')
    switch_type.create('C')
    switch_type.create('D')

    host_type.create('h1')
    host_type.create('h2')
    host_type.create('h3')
    host_type.create('h4')

    h1.linkTo(A, latency=1)
    h2.linkTo(B, latency=1)
    h3.linkTo(C, latency=1)
    h4.linkTo(D, latency=1)

    A.linkTo(B, latency=2)
    A.linkTo(C, latency=7)

    D.linkTo(B, latency=3)
    D.linkTo(C, latency=1)

    B.linkTo(C, latency=1)
