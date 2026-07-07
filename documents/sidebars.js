/** @type {import('@docusaurus/plugin-content-docs').SidebarsConfig} */
const sidebars = {
  guide: [
    {
      type: 'category',
      label: '入门指南',
      link: { type: 'doc', id: 'guide/intro' },
      items: [
        'guide/intro',
        'guide/build-guide',
        'guide/getting-started',
      ],
    },
  ],
  command: [
    'command/maskrom',
    'command/download',
    'command/upgrade',
    'command/ready',
    'command/version',
    'command/capability',
    'command/reset',
    'command/dump',
    'command/read',
    'command/write',
    'command/exec',
    'command/otp',
    'command/sn',
    'command/vs',
    'command/storage',
    'command/flash',
    'command/extra',
  ],
  examples: [
    'examples/maskrom-init',
    'examples/flash-write',
    'examples/maskrom-debug',
  ],
  reference: [
    'reference/chip-list',
    'reference/storage-types',
    'reference/capability-flags',
  ],
};

module.exports = sidebars;
